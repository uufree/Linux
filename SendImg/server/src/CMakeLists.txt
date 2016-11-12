include_directories(${PROJECT_SOURCE_DIR}/includ /usr/include/pthread)

set(CMAKE_C_COMPILER g++)

set(SERVER Main_Server.c)

add_definitions("-g -o -lpthread -std=c++11")

add_executable(ser ${SERVER})

target_link_libraries(ser SendImgLib pthread)

