#include<stdio.h>
#include<string.h>
#include<ctype.h>

int main(void){
	int a=01344,b=0x23942,c=0X8053945;
	float e=234982843.583049e-3;
	long long l=58234938204985;
	printf("%d %d %d\n",a-c,b++,--c);
	printf("%e\n",e+l);
	printf("%lld\n",l);
	return 0;
} 
