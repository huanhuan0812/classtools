cmake_minimum_required(VERSION 3.16)

# 设置项目名称和版本
project(rand-simple VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt6库
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)


# 设置自动处理UI、资源和MOC
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 添加可执行文件
add_executable(rand-simple
    main.cpp
)

# 链接Qt库
target_link_libraries(rand-simple PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# 设置应用程序属性
set_target_properties(rand-simple PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# 安装目标
install(TARGETS rand-simple
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)