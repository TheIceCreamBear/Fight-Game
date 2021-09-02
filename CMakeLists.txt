cmake_minimum_required(VERSION 3.6)   # CMake version check
project(Fight-Game C CXX)             # Create project "Fight-Game"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard

#add source folder so the program can be compiled at build folder
include_directories(${CMAKE_SOURCE_DIR})

#Headers
file(GLOB HEADERS
  *.h
  )

#Main
set(SOURCE_FILES src/tests.c src/gl.c src/logging.c src/main.c src/object.c src/room.c src/roomtype.c src/utils.c src/fight.c src/example-gl.c)
add_executable(Fight-Game ${SOURCE_FILES} ${HEADERS})

include_directories("include")

if (APPLE)
    include_directories(/usr/local/include)
endif()

#GLFW
if (APPLE)
    # do special stuff for macOS cause macOS sucks and doesnt have a brain
    find_package(glfw3 REQUIRED)
    include_directories(${GLFW_INCLUDE_DIRS})
    target_link_libraries(Fight-Game glfw)
else()
    # normal linux system
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})
    target_link_libraries(Fight-Game ${GLFW_LIBRARIES})
endif()

#OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(Fight-Game ${OPENGL_gl_LIBRARY})

#GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    if (APPLE)
        target_link_libraries(Fight-Game GLEW::GLEW)
    else()
        target_link_libraries(Fight-Game ${GLEW_LIBRARIES})
    endif()
endif()
