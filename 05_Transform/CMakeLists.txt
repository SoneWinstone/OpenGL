cmake_minimum_required(VERSION 3.8)
project(05_Transform)

set(CMAKE_CXX_STANDARD 11)

if (WIN32)
    set(SOURCE_FILES main.cpp win/glad.c)
#ELSEIF (APPLE)
#ELSEIF (UNIX)
else(WIN32)
    set(SOURCE_FILES main.cpp linux/glad.c)
endif (WIN32)

add_executable(05_Transform ${SOURCE_FILES})

if (UNIX)
    # Linux
    include_directories("/usr/local/include")
    set(OpenGL_LIBS "/usr/local/lib/libglfw.so.3")

    target_link_libraries(05_Transform ${OpenGL_LIBS} -ldl -lglfw -lGL)
else(UNIX)
    # Windows
    include_directories("D:/OpenGL/include")
    set(OpenGL_LIBS
            "D:/OpenGL/lib/libglfw3.a"
            )
    target_link_libraries(05_Transform ${OpenGL_LIBS} -lopengl32)
endif (UNIX)