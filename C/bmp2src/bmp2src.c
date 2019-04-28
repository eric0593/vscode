#include "stdio.h"
#include "stdlib.h"
#include <string.h>
 #include <ctype.h>

#pragma pack(2)
typedef struct _DIBFileHeader // DIB4
{
 unsigned short bfType ; // :"BM"0x4D42
 unsigned int bfSize ; // t*:
 unsigned short bfReserved1 ; // Y:0
 unsigned short bfReserved2 ; // Y:0
 unsigned int bfOffsetBits ; // DIB (-O
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


int WriteFile(char *filename,int offset)
{
	FILE *fp,*wfp;
	BMPInfo m_BMPInfo;
	char h_header[128]="__HX_SPLASH_";
	char wfile[128]="hx_splash_";
	unsigned int i=0;
	unsigned char c=0;
	
	strcat(wfile,filename);
	memset(wfile+strlen(wfile)-4,0x00,4);
	strcat(wfile,".h");
	
	fp = fopen(filename,"rb");	
	if(fp==NULL)
	{
		printf("file open failed\n");
		return -1;
	}

	wfp = fopen(wfile,"wb");	
	if(wfp==NULL)
	{
		printf("file open wfile failed %s\n",wfile);
		return -1;
	}
		
	for(i=0;i<strlen(filename)-4;i++)
	{
		h_header[strlen(h_header)]=toupper(filename[i]);
	}
	strcat(h_header,"_H__");
	fread(&m_BMPInfo,sizeof(BMPInfo),1,fp);
	printf("BMP FILE SIZE=%d\n",m_BMPInfo.bmpfileheader.bfSize);
	printf("BMP DATA OFFSET=%d\n",m_BMPInfo.bmpfileheader.bfOffsetBits);
	printf("BMP INFO SIZE=%d\n",m_BMPInfo.bmpinfoheader.biSize);
	printf("BMP INFO WIDTH=%d\n",m_BMPInfo.bmpinfoheader.biWidth);
	printf("BMP INFO HEIGHT=%d\n",m_BMPInfo.bmpinfoheader.biHeight);
	printf("BMP INFO BITCOUNT=%d\n",m_BMPInfo.bmpinfoheader.biBitCount);
	fputs("\#ifndef ",wfp);
	fputs(h_header,wfp);
	fputs("\n",wfp);
	
	fputs("\#define ",wfp);
	fputs(h_header,wfp);
	fputs("\n",wfp);

	if (m_BMPInfo.bmpinfoheader.biHeight<0)
	{
		m_BMPInfo.bmpinfoheader.biHeight=~m_BMPInfo.bmpinfoheader.biHeight+1;
	}
	if(m_BMPInfo.bmpinfoheader.biBitCount==16)
	{
		fputs("\#define SPLASH_IMAGE_WIDTH_MDPI ",wfp);
		fprintf(wfp,"%d",m_BMPInfo.bmpinfoheader.biHeight);
		fputc('\n',wfp);
		fputs("\#define SPLASH_IMAGE_HEIGHT_MDPI ",wfp);
		fprintf(wfp,"%d",m_BMPInfo.bmpinfoheader.biWidth);
		fputs("\n\n",wfp);
		fputs("static char imageBuffer[] = {\n\t",wfp);
	}
	else if(m_BMPInfo.bmpinfoheader.biBitCount==24)
	{
		fputs("\#define SPLASH_IMAGE_WIDTH_HDPI ",wfp);
		fprintf(wfp,"%d",m_BMPInfo.bmpinfoheader.biHeight);
		fputc('\n',wfp);
		fputs("\#define SPLASH_IMAGE_HEIGHT_HDPI ",wfp);
		fprintf(wfp,"%d",m_BMPInfo.bmpinfoheader.biWidth);	
		fputs("\n\n",wfp);	
		fputs("static char imageBuffer_rgb888[] = {\n\t",wfp);
	}
	for(i=0;i<m_BMPInfo.bmpfileheader.bfSize-m_BMPInfo.bmpfileheader.bfOffsetBits;i++)
	{
		c=fgetc(fp);
		fprintf(wfp,"0x%x,",c);
		if (!((i+1)%32))
			fputs("\n\t",wfp);
	}
	fputs("};\n",wfp);
	fputs("\#endif",wfp);
	printf("%s header file gen\n",wfile);
	fclose(fp);
	fclose(wfp);
	return 0;
}

int main(int argc,char** argv)
{
	int offset;
	if(argc<2)
	{
		printf("bmp2src bmpfile offset\n");
		printf("bmpfile: bmp file to covert\n");
		printf("offset: offset to be cut\n");
		return -1;
	}

	if (argc<3)
	{
		offset=54;
	}
	else	
		offset=atoi(argv[2]);
	
	WriteFile(argv[1],offset);
	
	return 0;
}
