#include<stdio.h>
main()
{
	int n,a=0,b=1;
	printf("输入一个整数\n");
	scanf("%d",&n);
	do
	{
	 a+=b;
	 b++;
	}while(b<=n);
	printf("%d",a);
	return 0;
	
	
}
