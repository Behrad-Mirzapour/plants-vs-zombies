cmake_minimum_required(VERSION 3.11)
project(Project)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)


add_executable(Project
        Src/main.c
        Src/helpers.c
        Src/data.c
        Src/game.c
        Src/drawing.c)

     
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)

if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    endif()
endif()


target_link_libraries(Project raylib)

# Copy Resources and Data directories to build directory
add_custom_command(TARGET Project POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resources" "$<TARGET_FILE_DIR:Project>/Resources"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:Project>/Data"
    COMMENT "Copying Resources and Data to build directory"
)

if (PLATFORM STREQUAL "Web")
    set_target_properties(Project PROPERTIES SUFFIX ".html")
    target_link_options(Project INTERFACE
            "SHELL:-s USE_GLFW=3"
            "SHELL:-s ASSERTIONS=1"
            "SHELL:-s WASM=1"
            "SHELL:-s ASYNCIFY"
            "SHELL:-s GL_ENABLE_GET_PROC_ADDRESS=1"
    )
endif()

#MACOS
if (APPLE)
    find_library(IOKIT IOKit)
    find_library(COCOA Cocoa)
    find_library(OPENGL OpenGL)
    target_link_libraries(Project ${IOKIT} ${COCOA} ${OPENGL})
endif()