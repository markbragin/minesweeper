cmake_minimum_required(VERSION 3.11)

project(minesweeper)

set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Check if raylib package is already installed
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED

# If there's none, fetch and build raylib
if (NOT raylib)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# Sources
set(
    SOURCES
    src/main.c
    src/grid.c
    src/drawings.c
    src/textures.c
    src/screen_gameplay.c
    src/screen_main_menu.c
    src/config.c
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Includes (-I)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})

# Libs (-l)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib m)

# Output binary
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    "${PROJECT_SOURCE_DIR}"
)
