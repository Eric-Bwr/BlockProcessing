cmake_minimum_required(VERSION 3.7)
project(Client)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

#Ill put them back in later, i want to fix other things first
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Werror -std=gnu2x")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -O2 -std=gnu++20 -Wno-deprecated-enum-enum-conversion")

#set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -std=gnu2x")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=gnu++20 -Wno-deprecated-enum-enum-conversion")

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCE_FILES_CLIENT "Client/BlockProcessing/*.cpp" "Client/BlockProcessing/*.h")
file(GLOB_RECURSE DEPENDENCIES_FILES "Dependencies/*.cpp" "Dependencies/*.c" "Dependencies/*.h")

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

add_executable(Client ${SOURCE_FILES_CLIENT} ${DEPENDENCIES_FILES} Dependencies/FastNoise/FastNoise.h Dependencies/FastNoise/FastNoise.cpp)

set_target_properties(Client PROPERTIES OUTPUT_NAME "Client")

target_compile_options(Client PRIVATE -w)

target_link_libraries(Client
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/FreeType/freetype.a
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/libglfw3.a
        wsock32
        ws2_32
        -static
        )

project(Server)

file(GLOB_RECURSE SOURCE_FILES_SERVER "Server/BlockProcessing/*.cpp" "Server/BlockProcessing/*.h")

include_directories(Server)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Server/Build)

add_executable(Server ${SOURCE_FILES_SERVER} ${DEPENDENCIES_FILES} Server/BlockProcessing/Server/PlayerManager.cpp Server/BlockProcessing/Server/PlayerManager.h)
set_target_properties(Server PROPERTIES OUTPUT_NAME "Server")

target_compile_options(Server PRIVATE -w)

target_link_libraries(Server
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/FreeType/freetype.a
        wsock32
        ws2_32
        -static
        )