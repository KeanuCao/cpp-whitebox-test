# gtest 已在顶层 CMakeLists.txt 里 include 过
add_library(logic_lib STATIC logic.cpp)
target_include_directories(logic_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_logic test_logic.cpp)
target_link_libraries(test_logic PRIVATE logic_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_logic)
