cmake_minimum_required(VERSION 3.16)
add_subdirectory(Dependencies)

project(Client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Client/Build)
file(GLOB_RECURSE SOURCE_FILES_CLIENT "Client/BlockProcessing/*.cpp" "Client/BlockProcessing/*.h")
include_directories(${PROJECT_NAME})

initDeps()

add_executable(${PROJECT_NAME} ${SOURCE_FILES_CLIENT})
afterExec()


project(Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Server/Build)
file(GLOB_RECURSE SOURCE_FILES_SERVER "Server/BlockProcessing/*.cpp" "Server/BlockProcessing/*.h")
include_directories(${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${SOURCE_FILES_SERVER})
afterExec()
