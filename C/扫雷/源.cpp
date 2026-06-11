// 最终修复版：EasyX图形化扫雷（解决字符类型报错 + 全功能）
#define _CRT_SECURE_NO_WARNINGS
#include <graphics.h>   // EasyX图形头文件
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>      // 引入TCHAR类型头文件

// 扫雷配置
#define ROW 9
#define COL 9
#define BLOCK_SIZE 40    // 每个格子大小
#define MINE_COUNT 10    // 雷的数量

// 两个棋盘
char mine[11][11] = { 0 };  // 后台雷盘
char show[11][11] = { 0 };  // 显示界面

// 1. 初始化棋盘
void InitBoard() {
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			mine[i][j] = '0';
			show[i][j] = '*';
		}
	}
}

// 2. 随机布置雷
void SetMine() {
	int count = MINE_COUNT;
	while (count) {
		int x = rand() % ROW + 1;
		int y = rand() % COL + 1;
		if (mine[x][y] == '0') {
			mine[x][y] = '1';
			count--;
		}
	}
}

// 3. 计算周围雷数
int GetMineCount(int x, int y) {
	return mine[x - 1][y - 1] + mine[x - 1][y] + mine[x - 1][y + 1] +
		mine[x][y - 1] + mine[x][y + 1] +
		mine[x + 1][y - 1] + mine[x + 1][y] + mine[x + 1][y + 1] - 8 * '0';
}

// 4. 递归展开空白区域
void ExpandBlank(int x, int y) {
	if (x < 1 || x > ROW || y < 1 || y > COL || show[x][y] != '*')
		return;

	int count = GetMineCount(x, y);
	if (count > 0) {
		show[x][y] = count + '0';
		return;
	}

	show[x][y] = ' ';
	// 递归8个方向
	ExpandBlank(x - 1, y - 1);
	ExpandBlank(x - 1, y);
	ExpandBlank(x - 1, y + 1);
	ExpandBlank(x, y - 1);
	ExpandBlank(x, y + 1);
	ExpandBlank(x + 1, y - 1);
	ExpandBlank(x + 1, y);
	ExpandBlank(x + 1, y + 1);
}

// 5. 判断游戏胜利
int IsWin() {
	int safe = 0;
	for (int i = 1; i <= ROW; i++) {
		for (int j = 1; j <= COL; j++) {
			if (show[i][j] != '*' && mine[i][j] == '0')
				safe++;
		}
	}
	return safe == ROW * COL - MINE_COUNT;
}

// 6. 绘制图形界面（核心修复字符类型）
void DrawGame() {
	cleardevice();  // 清屏

	for (int i = 1; i <= ROW; i++) {
		for (int j = 1; j <= COL; j++) {
			// 绘制格子
			setfillcolor(LIGHTGRAY);
			fillrectangle(j * BLOCK_SIZE, i * BLOCK_SIZE,
				(j + 1) * BLOCK_SIZE, (i + 1) * BLOCK_SIZE);

			// 修复：改用TCHAR类型适配EasyX
			setbkmode(TRANSPARENT);
			settextcolor(RED);
			TCHAR text[2] = { 0 };  // 替换char为TCHAR
			text[0] = show[i][j];    // 赋值要显示的字符
			// 直接传入TCHAR类型参数，解决类型不匹配
			outtextxy(j * BLOCK_SIZE + 12, i * BLOCK_SIZE + 8, text);
		}
	}
}

int main() {
	// 初始化随机数 + 图形窗口
	srand((unsigned int)time(NULL));
	initgraph(COL * BLOCK_SIZE + BLOCK_SIZE, ROW * BLOCK_SIZE + BLOCK_SIZE);

	// 游戏初始化
	InitBoard();
	SetMine();
	DrawGame();

	// 鼠标消息循环
	MOUSEMSG msg;
	while (1) {
		msg = GetMouseMsg();  // 获取鼠标操作

		// 左键点击
		if (msg.uMsg == WM_LBUTTONDOWN) {
			// 转换鼠标坐标为棋盘坐标
			int x = msg.y / BLOCK_SIZE;
			int y = msg.x / BLOCK_SIZE;

			// 越界判断
			if (x < 1 || x > ROW || y < 1 || y > COL)
				continue;

			// 1. 踩雷：游戏结束
			if (mine[x][y] == '1') {
				DrawGame();
				MessageBox(GetHWnd(), _T("💣 你被炸死了！游戏结束！"), _T("游戏结束"), MB_OK);
				break;
			}

			// 2. 正常点击：展开区域
			ExpandBlank(x, y);
			DrawGame();

			// 3. 判断胜利
			if (IsWin()) {
				DrawGame();
				MessageBox(GetHWnd(), _T("🎉 恭喜你！排雷成功！"), _T("胜利"), MB_OK);
				break;
			}
		}
	}

	closegraph();  // 关闭窗口
	return 0;
}