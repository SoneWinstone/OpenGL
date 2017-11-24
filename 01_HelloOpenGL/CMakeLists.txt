cmake_minimum_required(VERSION 3.8)
project(01_HelloOpenGL)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)

add_executable(01_HelloOpenGL ${SOURCE_FILES})

include_directories(F:/OpenGL/include)

set(OpenGL_LIBS
        "F:/OpenGL/lib/libglew32.a"
        "F:/OpenGL/lib/libglew32.dll.a"
        "F:/OpenGL/lib/glew32.lib"
        "F:/OpenGL/lib/glew32s.lib"
#        "F:/OpenGL/lib/OpenGL32.Lib"
        "F:/OpenGL/lib/libglfw3.a"
        "F:/OpenGL/lib/GLU32.Lib"
        )

target_link_libraries(01_HelloOpenGL ${OpenGL_LIBS} -lopengl32)
