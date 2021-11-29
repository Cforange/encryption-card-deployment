cmake_minimum_required(VERSION 3.17)
project(cg_new)

message(status "Project Directory: ${PROJECT_SOURCE_DIR}") #打印相关消息消息，可以用来检查相关设置是否正确
set(CMAKE_BUILD_TYPE DEBUG)                      #指定编译类型 DEBUG/RELEASE
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb")
#set(CMAKE_BUILD_TYPE "RELEASE")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g")
set(cmake_c_flags_debug "clang++") # 指定编译器-std=c++11
# add_compile_options( --target=x86_64-w64-mingw -std=gnu++11)  #添加编译选项，一些具体的编译选项大家可以自己查一下 ${DIR_SRC}
add_compile_options(-g -std=gnu++11)

include_directories("/Library/gurobi910/mac64/include")
include_directories("/Library/gurobi910/mac64/lib")
include_directories("/Library/gurobi910/gurobi.lic")
include_directories("/Users/faychan/CLionProjects/cg_new")
link_directories("/Library/gurobi910/mac64/lib")
link_directories("/Users/faychan/CLionProjects/cg_new")

ADD_EXECUTABLE(cg_new main.cpp heuristic.cpp def.h ksp.cpp rmp.cpp heur2.cpp createColumn.cpp ThreadPool.h)

add_definitions("-g")

target_link_libraries(cg_new /Library/gurobi910/mac64/lib/libgurobi_c++.a /Library/gurobi910/mac64/lib/libgurobi91.dylib)

target_compile_definitions(cg_new PRIVATE IL_STD=1)