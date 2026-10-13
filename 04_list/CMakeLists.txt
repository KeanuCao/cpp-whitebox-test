# 业务逻辑独立成静态库 —— 这是后面 gtest 能链接它的前提
add_library(list_lib STATIC list.cpp)
target_include_directories(list_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(04_list main.cpp)
target_link_libraries(04_list PRIVATE list_lib)
