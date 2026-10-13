add_library(fileio_lib STATIC fileio.cpp)
target_include_directories(fileio_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(05_fileio main.cpp)
target_link_libraries(05_fileio PRIVATE fileio_lib)
