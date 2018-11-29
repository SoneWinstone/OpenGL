cmake_minimum_required(VERSION 3.8)
project(03_Shader)

set(CMAKE_CXX_STANDARD 11)
set(PLATFORM Linux)

if (${PLATFORM} STREQUAL "Linux")
    # Linux
    message("Platform: Linux")
    set(SOURCE_FILES main.cpp linux/glad.c shader.h)
else(${PLATFORM} STREQUAL "Linux")
    # Windows
    message("Platform: Windows")
    set(SOURCE_FILES main.cpp win/glad.c shader.h)
endif (${PLATFORM} STREQUAL "Linux")

add_executable(03_Shader ${SOURCE_FILES})

if (${PLATFORM} STREQUAL "Linux")
    # Linux
    include_directories("/usr/local/include")
    set(OpenGL_LIBS "/usr/local/lib/libglfw.so.3")
    # dl:显式加载动态库的动态函数库，动态链接库显式调用时使用到dlclose函数
    # glfw:GLFW库
    # GL:OpenGL库
    target_link_libraries(03_Shader ${OpenGL_LIBS} -ldl -lglfw -lGL)
else(${PLATFORM} STREQUAL "Linux")
    # Windows
    include_directories("D:/OpenGL/include")
    set(OpenGL_LIBS
            "D:/OpenGL/lib/libglfw3.a"
            )
    target_link_libraries(03_Shader ${OpenGL_LIBS} -lopengl32)
endif (${PLATFORM} STREQUAL "Linux")