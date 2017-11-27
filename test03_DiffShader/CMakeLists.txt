cmake_minimum_required(VERSION 3.8)
project(test03_DiffShader)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(test03_DiffShader ${SOURCE_FILES})

include_directories(F:/OpenGL/include)

set(OpenGL_LIBS
        "F:/OpenGL/lib/libglew32.a"
        "F:/OpenGL/lib/libglew32.dll.a"
        "F:/OpenGL/lib/glew32.lib"
        "F:/OpenGL/lib/glew32s.lib"
        "F:/OpenGL/lib/libglfw3.a"
        "F:/OpenGL/lib/GLU32.Lib"
        )

target_link_libraries(test03_DiffShader ${OpenGL_LIBS} -lopengl32)