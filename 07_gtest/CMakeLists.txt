# gtest 已在顶层 CMakeLists.txt 里 include 过, 这里直接用 GTest::gtest_main
add_library(calc_lib STATIC calc.cpp)
target_include_directories(calc_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_calc test_calc.cpp)
target_link_libraries(test_calc PRIVATE calc_lib GTest::gtest_main)

# 把每个 TEST 注册成一个 ctest 用例, 这样才能 ctest -R 单独跑
include(GoogleTest)
gtest_discover_tests(test_calc)
