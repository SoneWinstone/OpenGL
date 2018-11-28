cmake_minimum_required(VERSION 3.8)
project(01_HelloOpenGL)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES #[[main.cpp]] glad_demo.cpp glad.c)

add_executable(01_HelloOpenGL ${SOURCE_FILES})

set(PLATFORM Linux)

if (${PLATFORM} STREQUAL "Linux")
    # Linux
    message("Platform: Linux")
    include_directories("/usr/local/include")
    set(OpenGL_LIBS "/usr/local/lib/libglfw.so.3")
    # dl:显式加载动态库的动态函数库，动态链接库显式调用时使用到dlclose函数
    # glfw:GLFW库
    # GL:OpenGL库
    target_link_libraries(01_HelloOpenGL ${OpenGL_LIBS} -ldl -lglfw -lGL)
else(${PLATFORM} STREQUAL "Linux")
    # Windows
    message("Platform: Windows")
    include_directories(F:/OpenGL/include)
    set(OpenGL_LIBS
            "F:/OpenGL/lib/libglew32.a"
            "F:/OpenGL/lib/libglew32.dll.a"
            "F:/OpenGL/lib/glew32.lib"
            "F:/OpenGL/lib/glew32s.lib"
            "F:/OpenGL/lib/libglfw3.a"
            "F:/OpenGL/lib/GLU32.Lib"
            )
    target_link_libraries(01_HelloOpenGL ${OpenGL_LIBS} -lopengl32)
endif (${PLATFORM} STREQUAL "Linux")
