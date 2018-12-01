cmake_minimum_required(VERSION 3.8)
project(04_Texture)

set(CMAKE_CXX_STANDARD 11)

if (WIN32)
    set(SOURCE_FILES main.cpp win/glad.c)
#ELSEIF (APPLE)
#ELSEIF (UNIX)
else(WIN32)
    set(SOURCE_FILES main.cpp linux/glad.c)
endif (WIN32)

add_executable(04_Texture ${SOURCE_FILES})

if (UNIX)
    # Linux
    include_directories("/usr/local/include")
    set(OpenGL_LIBS "/usr/local/lib/libglfw.so.3")

    target_link_libraries(04_Texture ${OpenGL_LIBS} -ldl -lglfw -lGL)
else(UNIX)
    # Windows
    include_directories("D:/OpenGL/include")
    set(OpenGL_LIBS
            "D:/OpenGL/lib/libglfw3.a"
            )
    target_link_libraries(04_Texture ${OpenGL_LIBS} -lopengl32)
endif (UNIX)