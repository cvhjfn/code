#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main() {
	int a = 0, n = 0;
	scanf("%d", &a);

	n++;
	a /= 10;
	while (a != 0) {
		a = a / 10;
		n++;
	}
	printf("%d\n", n);
	return 0;
}