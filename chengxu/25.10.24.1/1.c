#include<stdio.h>
int main(){
	double a,b,c;
	scanf("%lf %lf %lf",&a,&b,&c);
	if(a>b)
	b=a;
	if(c>b)
	b=c;
	printf("×î´óÖµ=%.7f\n",b);
	return 0;
}
