// ============================================================
//  知识点 8: 覆盖率实验 —— 行覆盖 100%, 分支覆盖却不满 100%
//
//  第一步: 先直接跑   bash scripts/coverage.sh
//          结果会是: logic.cpp 行覆盖 100%, 分支覆盖 90%
//          问题: 差的那 10% 是哪条分支? 为什么行覆盖发现不了它?
//
//  第二步: 打开本文件最下面的"动手改", 取消注释, 再跑一次
//          分支覆盖应该涨到 100%
// ============================================================
#include "logic.h"

#include <gtest/gtest.h>

// ---------- can_drive: 坑就在这个函数里 ----------
TEST(CanDrive, AdultWithLicense) {
    // age>=18 为真 -> 继续求值 has_license -> 为真
    EXPECT_TRUE(logic::can_drive(20, true));
}


TEST(CanDrive, AdultWithOutLicense) {
    // age>=18 为真 -> 继续求值 has_license -> 为假
    EXPECT_FALSE(logic::can_drive(20, false));
}

TEST(CanDrive, Minor) {
    // age>=18 为假 -> 短路! 后面的 has_license 根本没被求值
    EXPECT_FALSE(logic::can_drive(15, true));
}

// 动手改: 取消下面这个用例的注释, 再跑一次 coverage.sh
// 想一想: 为什么少了它, 行覆盖能到 100%, 分支覆盖却到不了?
//
// TEST(CanDrive, AdultWithoutLicense) {
//     // 补上 has_license 为假的那条分支
//     EXPECT_FALSE(logic::can_drive(20, false));
// }

// ---------- classify: 每条 return 都要有人走到 ----------
TEST(Classify, Negative)  { EXPECT_EQ(logic::classify(-1), -1); }
TEST(Classify, Fail)      { EXPECT_EQ(logic::classify(59),  0); }
TEST(Classify, Pass)      { EXPECT_EQ(logic::classify(60),  1); }
TEST(Classify, Excellent) { EXPECT_EQ(logic::classify(95),  2); }

// ---------- 对照组: 没有分支, 想不满都难 ----------
TEST(Square, Basic) {
    EXPECT_EQ(logic::square(0),  0);
    EXPECT_EQ(logic::square(4),  16);
    EXPECT_EQ(logic::square(-3), 9);
}
