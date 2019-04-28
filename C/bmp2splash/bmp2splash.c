#include "stdio.h"
#include "stdlib.h"
#include <string.h>
 #include <ctype.h>

#pragma pack(2)
typedef struct _DIBFileHeader // DIB4
{
 unsigned short bfType ; // :"BM"0x4D42 bit 0-1
 unsigned int bfSize ; // 文件大小bit 2-5
 unsigned short bfReserved1 ; // Y:0
 unsigned short bfReserved2 ; // Y:0
 unsigned int bfOffsetBits ; // bmp数据的偏移地址 bit 10-13
} DIBFileHeader;
 
typedef struct _DIBInfoHeader
{
 unsigned int biSize ; // S' = 40
 unsigned int biWidth ; // G
 int  biHeight ; // G
 unsigned short  biPlanes ; // = 1
 unsigned short  biBitCount ; // r(1, 4, 8, 16, 24, or 32)
 unsigned int  biCompression ; // )
 unsigned int  biSizeImage ; // Wp
 unsigned int  biXPelsPerMeter ; // horizontal resolution
 unsigned int  biYPelsPerMeter ; // vertical resolution
 unsigned int  biClrUsed ; // rp
 unsigned int  biClrImportant ; // rp
}DIBInfoHeader ;

typedef struct _BMPInfo
{
	DIBFileHeader bmpfileheader;
	DIBInfoHeader bmpinfoheader;
}BMPInfo;


int WriteFile(char *filename)
{
	FILE *fp,*wfp;
	BMPInfo m_BMPInfo;
	char splash_header[512] = "SPLASH!!";
	char wfile[128]="splash_";
	unsigned int i=0;
	unsigned char c=0;
	unsigned bmpDataSize = 0;
	//Get write file name
	strcat(wfile,filename);
	memset(wfile+strlen(wfile)-4,0x00,4);
	strcat(wfile,".img");
	
	fp = fopen(filename,"rb");	
	if(fp==NULL)
	{
		printf("Bmp file %s open failed\n",filename);
		return -1;
	}

	wfp = fopen(wfile,"wb");
	if(wfp==NULL)
	{
		printf("splash file %s open failed\n",wfile);
		return -1;
	}
		
	fread(&m_BMPInfo,sizeof(BMPInfo),1,fp);
	printf("BMP FILE SIZE=%d\n",m_BMPInfo.bmpfileheader.bfSize);
	printf("BMP DATA OFFSET=%d\n",m_BMPInfo.bmpfileheader.bfOffsetBits);
	printf("BMP INFO SIZE=%d\n",m_BMPInfo.bmpinfoheader.biSize);
	printf("BMP INFO WIDTH=%d\n",m_BMPInfo.bmpinfoheader.biWidth);
	printf("BMP INFO HEIGHT=%d\n",m_BMPInfo.bmpinfoheader.biHeight);
	printf("BMP INFO BITCOUNT=%d\n",m_BMPInfo.bmpinfoheader.biBitCount);

	//biSizeImage sometime is 0, use bmpDataSize instend
	bmpDataSize = m_BMPInfo.bmpfileheader.bfSize - m_BMPInfo.bmpfileheader.bfOffsetBits;
	printf("BMP DATA SIZE=%d\n",bmpDataSize);	
	//倒序
	if (m_BMPInfo.bmpinfoheader.biHeight<0)
	{
		m_BMPInfo.bmpinfoheader.biHeight=~m_BMPInfo.bmpinfoheader.biHeight+1;
	}

	//width
	splash_header[8] = ( m_BMPInfo.bmpinfoheader.biWidth        & 0xFF);
    splash_header[9] = ((m_BMPInfo.bmpinfoheader.biWidth >> 8 ) & 0xFF);
    splash_header[10]= ((m_BMPInfo.bmpinfoheader.biWidth >> 16) & 0xFF);
    splash_header[11]= ((m_BMPInfo.bmpinfoheader.biWidth >> 24) & 0xFF);

    //height
    splash_header[12]= ( m_BMPInfo.bmpinfoheader.biHeight        & 0xFF);
    splash_header[13]= ((m_BMPInfo.bmpinfoheader.biHeight >>  8) & 0xFF);
    splash_header[14]= ((m_BMPInfo.bmpinfoheader.biHeight >> 16) & 0xFF);
    splash_header[15]= ((m_BMPInfo.bmpinfoheader.biHeight >> 24) & 0xFF);

	fwrite(splash_header,1,sizeof(splash_header),wfp);


	for(i=0;i<bmpDataSize;i++)
	{
		c=fgetc(fp);
		fputc(c,wfp);

	}
	
	printf("%s splash file gen\n",wfile);
	fclose(fp);
	fclose(wfp);
	return 0;
}

int main(int argc,char** argv)
{
	if(argc<2)
	{
		printf("bmp2src bmpfile\n");
		printf("bmpfile: bmp file to covert\n");
		return -1;
	}

	
	WriteFile(argv[1]);
	
	return 0;
}
