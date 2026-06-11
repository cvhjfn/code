#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

int main() {
    int num;
    cin >> num;
    if (num % 2 == 1) {
        cout << "odd number" << endl;
    }
    else {
        cout << "even number" << endl;
    }
    return 0;
}