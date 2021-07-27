cmake_minimum_required(VERSION 3.7)
project(BlockProcessing)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCE_FILES "BlockProcessing/*.cpp" "BlockProcessing/*.h")

include_directories(Dependencies)
include_directories(${PROJECT_NAME} "Dependencies/FreeType/include")
include_directories(${PROJECT_NAME} "Dependencies/OpenGL/include")
include_directories(${PROJECT_NAME} "Dependencies/Math/include")
include_directories(${PROJECT_NAME} "Dependencies/GLFW/include")
include_directories(${PROJECT_NAME} "Dependencies/UI/include")
include_directories(${PROJECT_NAME} "Dependencies/FastNoise")
include_directories(${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} Dependencies/FastNoise/FastNoise.h Dependencies/FastNoise/FastNoise.cpp BlockProcessing/Game/Interface/Messages.h BlockProcessing/Game/PostProcess/PostProcessShader.h BlockProcessing/Game/Interface/CrosshairInterface/CrosshairInterface.cpp BlockProcessing/Game/Interface/CrosshairInterface/CrosshairInterface.h)

target_link_libraries(${PROJECT_NAME}
        ${CMAKE_CURRENT_LIST_DIR}/Dependencies/UI/UI.a
        ${CMAKE_CURRENT_LIST_DIR}/Dependencies/FreeType/freetype.a
        ${CMAKE_CURRENT_LIST_DIR}/Dependencies/GLFW/libglfw3.a
        ${CMAKE_CURRENT_LIST_DIR}/Dependencies/OpenGL/OpenGL.a
        ${CMAKE_CURRENT_LIST_DIR}/Dependencies/Math/Math.a
        -static
        )