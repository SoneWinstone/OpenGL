cmake_minimum_required(VERSION 3.8)
project(01_Color)

set(CMAKE_CXX_STANDARD 11)

if (WIN32)
    set(SOURCE_FILES main.cpp win/glad.c)
else(WIN32)
    set(SOURCE_FILES main.cpp linux/glad.c)
endif (WIN32)

add_executable(01_Color ${SOURCE_FILES})

if (UNIX)
    # Linux
    include_directories("/usr/local/include")
    set(OpenGL_LIBS "/usr/local/lib/libglfw.so.3")

    target_link_libraries(01_Color ${OpenGL_LIBS} -ldl -lglfw -lGL)
else(UNIX)
    # Windows
    include_directories("D:/OpenGL/include")
    set(OpenGL_LIBS
            "D:/OpenGL/lib/libglfw3.a"
            )
    target_link_libraries(01_Color ${OpenGL_LIBS} -lopengl32)
endif (UNIX)