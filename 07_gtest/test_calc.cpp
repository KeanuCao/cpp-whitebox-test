// ============================================================
//  知识点 7: GoogleTest 白盒测试入门
//  跑法:  cd build && ctest --output-on-failure
//         cd build && ctest -R IsPrime          # 只跑名字含 IsPrime 的
//         ./build/bin/test_calc                 # 也可以直接跑
//         ./build/bin/test_calc --gtest_filter=CalcDiv.*
// ============================================================
#include "calc.h"

#include <gtest/gtest.h>

// ---------- 1. 最简单的 TEST ----------
// TEST(测试套件名, 用例名) —— 名字里不能有下划线以外的特殊字符
TEST(CalcAdd, HandlesPositiveNumbers) {
    EXPECT_EQ(calc::add(2, 3), 5);
}

TEST(CalcAdd, HandlesNegativeAndZero) {
    EXPECT_EQ(calc::add(-2, -3), -5);
    EXPECT_EQ(calc::add(0, 0), 0);
    EXPECT_EQ(calc::add(-5, 5), 0);
}

// 动手改(第 1 个实验): 取消下面的注释, 重编, 观察 EXPECT_ 和 ASSERT_ 的区别
//   EXPECT_EQ 失败 -> 打印失败信息, 但用例继续往下跑
//   ASSERT_EQ 失败 -> 当前用例立刻 return, 后面的断言不再执行
//
// TEST(CalcAdd, ExpectVsAssert) {
//     EXPECT_EQ(calc::add(1, 1), 2);
//     EXPECT_EQ(calc::add(1, 1), 99);   // 故意失败: 后面的还跑
//     ASSERT_EQ(calc::add(1, 1), 99);   // 故意失败: 后面的不跑了
//     EXPECT_EQ(calc::add(2, 2), 4);    // 这行不会被执行
// }

TEST(CalcSub, Basic) {
    EXPECT_EQ(calc::sub(10, 4), 6);
    EXPECT_NE(calc::sub(10, 4), 7);   // EXPECT_NE: 不相等
}

TEST(CalcMul, Basic) {
    EXPECT_EQ(calc::mul(6, 7), 42);
    EXPECT_EQ(calc::mul(-6, 7), -42);
}

// ---------- 2. 测"抛异常"这件事 ----------
TEST(CalcDiv, TruncatesTowardZero) {
    EXPECT_EQ(calc::div(7, 2), 3);     // C++ 整数除法向零截断, 不是四舍五入
    EXPECT_EQ(calc::div(-7, 2), -3);
}

TEST(CalcDiv, ThrowsOnZero) {
    EXPECT_THROW(calc::div(1, 0), std::invalid_argument);
    // EXPECT_NO_THROW: 反过来, 确认不抛
    EXPECT_NO_THROW(calc::div(1, 1));
}

// ---------- 3. 布尔断言 ----------
TEST(CalcIsPrime, SmallNumbers) {
    EXPECT_FALSE(calc::is_prime(-7));  // 边界: 负数
    EXPECT_FALSE(calc::is_prime(0));   // 边界: 0
    EXPECT_FALSE(calc::is_prime(1));   // 边界: 1  (最容易写错的边界)
    EXPECT_TRUE (calc::is_prime(2));   // 边界: 最小的素数
    EXPECT_TRUE (calc::is_prime(3));
    EXPECT_FALSE(calc::is_prime(4));
    EXPECT_TRUE (calc::is_prime(97));
    EXPECT_FALSE(calc::is_prime(100));
}

// ---------- 4. 参数化测试 TEST_P: 一份代码跑一堆输入 ----------
// 步骤: 定义夹具类 -> INSTANTIATE_TEST_SUITE_P 喂数据 -> TEST_P 写断言
class IsPrimeParam : public ::testing::TestWithParam<std::tuple<int, bool>> {};

TEST_P(IsPrimeParam, MatchesTable) {
    const auto [n, expected] = GetParam();   // C++17 结构化绑定
    EXPECT_EQ(calc::is_prime(n), expected) << "当 n = " << n << " 时判错了";
}
// 上面那个 << 是 gtest 的贴心设计: 失败时把 n 的值一起打出来

INSTANTIATE_TEST_SUITE_P(
    PrimeTable, IsPrimeParam,
    ::testing::Values(
        std::make_tuple(  -1, false),
        std::make_tuple(   0, false),
        std::make_tuple(   1, false),
        std::make_tuple(   2, true ),
        std::make_tuple(   9, false),
        std::make_tuple(  25, false),
        std::make_tuple(  49, false),   // 7*7, 专治 "只试到 sqrt(n)" 写错
        std::make_tuple( 101, true ),
        std::make_tuple(1000, false)));

// ---------- 5. 自己动手加一组 ----------
TEST(CalcGcd, Basic) {
    EXPECT_EQ(calc::gcd(12, 18), 6);
    EXPECT_EQ(calc::gcd(18, 12), 6);   // 交换律
    EXPECT_EQ(calc::gcd(7, 13), 1);    // 互质
    EXPECT_EQ(calc::gcd(0, 0), 0);     // 边界
    EXPECT_EQ(calc::gcd(-12, 18), 6);  // 负数
    // 动手改: 把下面这行的 5 改成 6, 重编, 看 ctest 怎么报错
    EXPECT_EQ(calc::gcd(15, 20), 5);
}
