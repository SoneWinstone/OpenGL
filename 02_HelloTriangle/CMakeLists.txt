cmake_minimum_required(VERSION 3.8)
project(02_HelloTriangle)

set(CMAKE_CXX_STANDARD 11)

if (WIN32)
    message("Platform: Windows")
    set(PLATFORM Windows)
#ELSEIF (APPLE)
#ELSEIF (UNIX)
ELSE(WIN32)
    message("Platform: Linux")
    set(PLATFORM Linux)
endif (WIN32)

if (${PLATFORM} STREQUAL "Linux")
    # Linux
    set(SOURCE_FILES main.cpp linux/glad.c #[[glew.cpp]])
else(${PLATFORM} STREQUAL "Linux")
    # Windows
    set(SOURCE_FILES main.cpp win/glad.c #[[glew.cpp]])
endif (${PLATFORM} STREQUAL "Linux")

add_executable(02_HelloTriangle ${SOURCE_FILES})

if (${PLATFORM} STREQUAL "Linux")
    # Linux
    include_directories("/usr/local/include")
    set(OpenGL_LIBS "/usr/local/lib/libglfw.so.3")
    # dl:显式加载动态库的动态函数库，动态链接库显式调用时使用到dlclose函数
    # glfw:GLFW库
    # GL:OpenGL库
    target_link_libraries(02_HelloTriangle ${OpenGL_LIBS} -ldl -lglfw -lGL)
else(${PLATFORM} STREQUAL "Linux")
    # Windows
    include_directories("D:/OpenGL/include")
    set(OpenGL_LIBS
            "D:/OpenGL/lib/libglfw3.a"
            )
    target_link_libraries(02_HelloTriangle ${OpenGL_LIBS} -lopengl32)
endif (${PLATFORM} STREQUAL "Linux")