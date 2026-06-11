#define _CRT_SECURE_NO_WARNINGS

#include"game.h"

void menu()
{
	printf("***********************\n");
	printf("**** 1. play game ****\n");
	printf("**** 0. exit      ****\n");
	printf("***********************\n");
}
void game()
{
	char mine[ROWS][COLS] = { 0 };//存储雷的信息
	char show[ROWS][COLS] = { 0 };//存储显示的信息

	//初始化棋盘
	InitBoard(mine, ROWS, COLS,'0');
	InitBoard( show, ROWS, COLS,'*');

	//显示棋盘
	DisplayBoard(show,ROW,COL);

	//布置雷
	SetMine(mine, ROW, COL);

	//显示布置好的雷

}
void test()
{
	int input = 0;
	srand(unsigned int)time(NULL);
	do
	{
		menu();
		printf("请选择:>");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			printf("游戏开始\n");
			break;
		case 0:
			printf("退出游戏\n");
			break;
		default:
			printf("选择错误，请重新选择\n");
			break;
		}
	} while (input);
}
int main()
{
	 
	return 0;
}


		
