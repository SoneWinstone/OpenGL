cmake_minimum_required(VERSION 3.9)
project(01_Assimp)

set(CMAKE_CXX_STATNDARD 11)

add_executable(01_Assimp main.cpp glad.c Mesh.cpp Mesh.h Model.cpp Model.h)

include_directories("/usr/local/include")

set(OpenGL_LIBS "/usr/local/lib/libglfw.so.3")

# Windows
#target_link_libraries(01_Assimp ${OpenGL_LIBS} -lopengl32)

# Linux
target_link_libraries(01_Assimp ${OpenGL_LIBS} dl glfw GL)
