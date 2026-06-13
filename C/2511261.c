#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{srand((unsigned int)time(0));
 int count=0,b=0;
 int a=rand()%100+1;
 do
 {printf("请输入一个数字\n",b);
  count++;
  scanf("%d",&b);
  if(a>b){printf("您猜小了");
  }else if(a<b)
  {printf("您猜大了");
  }
 }while(a!=b);
 printf("您猜对了\n%d",count);
 return  0;
}
