cmake_minimum_required(VERSION 3.10)
set(CMAKE_BUILD_TYPE Debug)
project(GameProject)

set(CMAKE_CXX_STANDARD 17)

set(OXYGINE_SDL ON CACHE BOOL "" FORCE)
set(OXYGINE_build_examples OFF CACHE BOOL "" FORCE)
set(OXYGINE_build_tools OFF CACHE BOOL "" FORCE)

add_subdirectory(oxygine-framework)
include_directories(oxygine-framework/oxygine/src)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})  #для поиска заголовочных файлов

add_executable(GameProject 
    src/main.cpp 
    src/Game.cpp
)

target_link_libraries(GameProject
    PRIVATE
    oxygine-framework
    
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    GL
    Threads::Threads
    dl
    
    ${JPEG_LIBRARIES}
    ${PNG_LIBRARIES}
    ${ZLIB_LIBRARIES}
)