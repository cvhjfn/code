#include<stdio.h>
int main(){
	int a,b,c,d;
	scanf("%d",&a);
	b=a/100;
	c=(a%100)/10;
	d=a%10;
	printf("百位数字a=%d,十位数字b=%d,个位数字c=%d",b,c,d);
	return 0;
}
