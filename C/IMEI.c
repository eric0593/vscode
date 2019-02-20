#include "stdio.h"
#include <stdlib.h>
typedef unsigned long long uint64_t;
#define IMEI_SIZE 15

uint64_t strtoint64(char *str)
{
	int i = 0;
	uint64_t n = 0;
	int flag = 1;
 
	if(str[0] == '-')
	{
		i = 1;
		flag = -1;
	}
 
	for(; str[i] != '\0' ; i++)
	{
		//assert(str[i] >= '0' && str[i] <= '9');
		//循环转换
		n = str[i] - '0' + n*10;
	}
	return n*flag;
}

void int64tostr(uint64_t val, char * buf)
{	
	int i=0,len=0;
	//无符号64位整数最多20位
	char temp[20+1];

	while(val)
	{
		temp[i++] = (val%10) +'0';
		val = val/10;
	}
	len = i;
	while(i--)
	{
		buf[len-i-1] = temp[i];
	}
}


char  GetIMEICheckDigit(char * imei)
{
	int			i;
	int			sum1=0,sum2=0,total=0;
	int			temp=0;

	for( i=0; i<14; i++ )
	{
		if((i%2)==0)
		{
			sum1 = sum1 + imei[i] - '0';
		}
		else
		{
			temp = (imei[i]-'0')*2;
			if( temp < 10 )
			{
				sum2 = sum2 + temp;
			}
			else
			{
				sum2 = sum2 + 1 + temp - 10;
			}
		}
	}

	total = sum1 + sum2;

	if( (total%10) == 0 )
	{
		return '0';
	}
	else
	{
		return (((total/10) * 10) + 10 - total+'0');
	}
}

int main(int argc, char* argv[])
{
	int count = 0 , i=0;
	uint64_t start = 0,imei;
	char buff[IMEI_SIZE+1];

	#if 1
	if (argc < 3)
	{
		printf("Parameter input error!\n");
		printf("Correct input: IMEI 86671303877757 100");
		return -1;
	}
	#endif

	count = atoi(argv[2]);
	start = strtoint64(argv[1]);
	printf("Count %d,start with %s\n",count,argv[1]);

	for (i=0;i<count;i++)
	{
		imei = start + i;
		int64tostr(imei,buff);
		printf("%s%c\n",buff,GetIMEICheckDigit(buff));
	}      
}

