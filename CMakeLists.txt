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
file(GLOB_RECURSE DEPENDENCIES_FILES_CLIENT "Dependencies/*.cpp" "Dependencies/*.c" "Dependencies/*.h")

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

add_executable(Client ${SOURCE_FILES_CLIENT} ${DEPENDENCIES_FILES_CLIENT} Dependencies/FastNoise/FastNoise.h Dependencies/FastNoise/FastNoise.cpp Client/BlockProcessing/Framework/Engine/Performance/GPUSpeedTester.h Client/BlockProcessing/Framework/Engine/Performance/SpeedTester.h)
set_target_properties(Client PROPERTIES OUTPUT_NAME "Client")

target_compile_options(Client PRIVATE -w)

target_link_libraries(Client
      #  ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/UI/UI.a
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/FreeType/freetype.a
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/libglfw3.a
        #${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/OpenGL/OpenGL.a
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Network/Network.a
        wsock32
        ws2_32
        -static
        )
