#include "calc.h"

namespace calc {

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

int div(int a, int b) {
    if (b == 0) throw std::invalid_argument("division by zero");
    return a / b;
}

bool is_prime(int n) {
    if (n < 2) return false;          // 分支 1: 0, 1, 负数
    if (n == 2) return true;          // 分支 2: 唯一的偶素数
    if (n % 2 == 0) return false;     // 分支 3: 其他偶数
    for (int d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false; // 分支 4: 找到因子
    }
    return true;                      // 分支 5: 循环走完
}

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

}  // namespace calc
