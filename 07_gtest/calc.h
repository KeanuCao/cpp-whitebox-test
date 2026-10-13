// ============================================================
//  知识点 7: 被测代码 (SUT, System Under Test)
//  注意: 这里没有任何 main, 也没打印任何东西 —— 这就是"可测"的样子
// ============================================================
#pragma once
#include <stdexcept>

namespace calc {

int add(int a, int b);
int sub(int a, int b);
int mul(int a, int b);

// b == 0 时抛 std::invalid_argument —— 用来演示 EXPECT_THROW
int div(int a, int b);

// 参数化测试的好素材: 分支多、边界清晰
bool is_prime(int n);

// 辗转相除; gcd(0,0) 定义为 0
int gcd(int a, int b);

}  // namespace calc
