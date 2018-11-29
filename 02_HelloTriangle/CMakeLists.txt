cmake_minimum_required(VERSION 3.8)
project(02_HelloTriangle)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp glad.c #[[glew.cpp]])
add_executable(02_HelloTriangle ${SOURCE_FILES})

set(PLATFORM Windows)

if (${PLATFORM} STREQUAL "Linux")
    # Linux
    message("Platform: Linux")
    include_directories("/usr/local/include")
    set(OpenGL_LIBS "/usr/local/lib/libglfw.so.3")
    # dl:显式加载动态库的动态函数库，动态链接库显式调用时使用到dlclose函数
    # glfw:GLFW库
    # GL:OpenGL库
    target_link_libraries(02_HelloTriangle ${OpenGL_LIBS} -ldl -lglfw -lGL)
else(${PLATFORM} STREQUAL "Linux")
    # Windows
    message("Platform: Windows")
    include_directories("D:/OpenGL/include")
    set(OpenGL_LIBS
            "D:/OpenGL/lib/libglfw3.a"
            )
    target_link_libraries(02_HelloTriangle ${OpenGL_LIBS} -lopengl32)
endif (${PLATFORM} STREQUAL "Linux")