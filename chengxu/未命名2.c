#include <stdio.h>

int main() {
    // 定义变量，n存储人数，sum存储总分，count统计高于平均分的人数
    int n, sum = 0, count = 0;
    // 定义数组存储成绩（n<100，所以数组大小设为100足够）
    int scores[100];
    // 平均分用浮点型，避免整数除法导致精度丢失
    float average;

    // 第一步：读取学生人数
    scanf("%d", &n);

    // 第二步：读取n个成绩并计算总分
    for (int i = 0; i < n; i++) {
        scanf("%d", &scores[i]);
        sum += scores[i]; // 累加计算总分
    }

    // 第三步：计算平均分
    average = (float)sum / n; // 强制类型转换为浮点型

    // 第四步：统计高于平均分的人数
    for (int i = 0; i < n; i++) {
        if (scores[i] > average) {
            count++;
        }
    }

    // 第五步：输出结果
    printf("%d\n", count);

    return 0;
}
