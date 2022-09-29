cmake_minimum_required(VERSION 3.16)
add_subdirectory(Dependencies)

project(Client)

set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE SOURCE_FILES_CLIENT "Client/BlockProcessing/*.cpp" "Client/BlockProcessing/*.h")
include_directories(${PROJECT_NAME})

initDeps()

add_executable(${PROJECT_NAME} ${SOURCE_FILES_CLIENT})
afterExec()


project(Server)

set(CMAKE_CXX_STANDARD 20)
file(GLOB_RECURSE SOURCE_FILES_SERVER "Server/BlockProcessing/*.cpp" "Server/BlockProcessing/*.h")
include_directories(${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${SOURCE_FILES_SERVER})
afterExec()
