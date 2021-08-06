cmake_minimum_required(VERSION 3.7)
project(Client)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCE_FILES_CLIENT "Client/BlockProcessing/*.cpp" "Client/BlockProcessing/*.h")

include_directories(Dependencies)
include_directories("Dependencies/Network/include")
include_directories("Dependencies/FreeType/include")
include_directories("Dependencies/OpenGL/include")
include_directories("Dependencies/Math/include")
include_directories("Dependencies/GLFW/include")
include_directories("Dependencies/UI/include")
include_directories("Dependencies/FastNoise")
include_directories(Client)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Client/Build)

add_executable(Client ${SOURCE_FILES_CLIENT} Dependencies/FastNoise/FastNoise.h Dependencies/FastNoise/FastNoise.cpp)

target_link_libraries(Client
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/UI/UI.a
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/FreeType/freetype.a
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/libglfw3.a
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/OpenGL/OpenGL.a
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Math/Math.a
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Network/Network.a
        wsock32
        ws2_32
        -static
        )

project(Server)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCE_FILES_SERVER "Server/BlockProcessing/*.cpp" "Server/BlockProcessing/*.h")

include_directories(Dependencies)
include_directories("Dependencies/Network/include")
include_directories("Dependencies/Math/include")
include_directories("Dependencies/FastNoise")
include_directories(Server)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Server/Build)

add_executable(Server ${SOURCE_FILES_SERVER} Dependencies/FastNoise/FastNoise.h Dependencies/FastNoise/FastNoise.cpp Server/BlockProcessing/BlockProcessing.cpp)

target_link_libraries(Server
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Math/Math.a
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Network/Network.a
        wsock32
        ws2_32
        -static
        )