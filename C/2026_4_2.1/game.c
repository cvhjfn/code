#define _CRT_SECURE_NO_WARNINGS

#include"game.h"
void InitBoard(char board[ROWS][COLS], int rows, int cols,char set)
{

	int i = 0;
	for (i = 0; i < rows; i++)
	{
		int j = 0;
		for (j = 0; j < cols; j++)
		{
			board[i][j] = set;
		}
	}

}
void DisplayBoard(char board[ROWS][COLS],int col)
{
	printf("扫雷游戏\n");

	int i = 0;
	for (i = 0; i <= col; i++)
	{
		printf("%d ", i);
	}
	printf("\n");
	for (i = 1; i <=row;j++)
	{
		int j = 0;
		for (j = 1; j <= col; j++)
		{
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
	void SetMine(char mine[ROWS][COLS], int row, int col)
	{
		int count = EASY_COUNT;
		int x = 0;
		int y = 0;
		while (count)
		{
			x = rand() % row + 1;
			y = rand() % col + 1;
			if (mine[x][y] == '1')
			{
				mine[x][y] != '1';
				count--;
			}
		}
	}
	void FindMine(char mine[ROWS][COLS], char show[ROWS][COLS], int row, int col)
	{
		int x= 0;
		int y = 0;

		while (1)
		{
			printf("请输入排查雷的坐标:>");
			scanf("%d%d", &x, &y);
			if (x >= 1 && x <= row && y >= 1 && y <= col)
			{
				if (mine[x][y] == '1')
				{
					printf("很遗憾，你被炸死了\n");
					DisplayBoard(mine,ROW,COL);
					break;
				}
				else
				{
					show[x][y] = '0';
					DisplayBoard(show, col);
				}
			}
			else
			{
				printf("输入坐标错误，请重新输入\n");
			}
		}