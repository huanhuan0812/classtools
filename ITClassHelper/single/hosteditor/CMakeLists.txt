cmake_minimum_required(VERSION 3.31)
project(hosteditor)

#set(CMAKE_CXX_STANDARD 23)

# 设置ICU路径
#set(ICU_ROOT "D:/managers/mingw-w64-x86_64-icu-77.1-1-any.pkg/mingw64")

# 定义目标
add_executable(hosteditor main.cpp)

# 包含目录
#target_include_directories(hosteditor PRIVATE "${ICU_ROOT}/include")

# 链接目录
#target_link_directories(hosteditor PRIVATE "${ICU_ROOT}/lib")

# 完整ICU库链接（关键修改）
#target_link_libraries(hosteditor PRIVATE icuuc icuin icudt icuio  # 添加缺失的IO库 icutu  # 添加工具库)

# 关键编译定义
#target_compile_definitions(hosteditor PRIVATE U_DISABLE_RENAMING=1 U_STATIC_IMPLEMENTATION)

# 静态链接配置
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -licudt")