include_directories(${PROJECT_SOURCE_DIR}/includ)

set(CMAKE_C_COMPILER g++)

set(CLIENT Main_Client.c)

add_definitions("-g -o -std=c++11")

add_executable(cli ${CLIENT})

target_link_libraries(cli SendImgLib)
