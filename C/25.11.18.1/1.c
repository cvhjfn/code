#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main() {
	int a, b;
	float c, d;
	scanf("%d", &a);
	b = a - 5000;
	if (b <= 3000)c = 0.03 * b, d = a - c,
		printf("个人所得税是：%.2f""个人所得是：%.2f", c, d);
	else if (b > 3000 && b <= 12000)c = 0.03 * 3000 + 0.1 * (b - 3000), d = a - c,
		printf("个人所得税是：%.2f""个人所得是：%.2f", c, d);
	else if (b > 12000)c = 0.03 * 3000 + 0.1 * 9000 + 0.2 * (b - 12000), d = a - c,
		printf("个人所得税是:%.2f""个人所得是：%.2f", c, d);
	return 0;
}