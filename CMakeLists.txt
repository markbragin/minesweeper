cmake_minimum_required(VERSION 3.0)

project(minesweeper)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Check if raylib is already installed
find_library(raylib raylib)

# Download raylib if not installed yet
if (NOT raylib)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
    )
    set(BUILD_EXAMPLES OFF CACHE INTERNAL "")
    FetchContent_MakeAvailable(raylib)
endif()

set(
    SOURCES
    src/main.c
    src/grid.c
    src/drawings.c
    src/textures.c
    src/screens/screen_gameplay.c
    src/screens/screen_main_menu.c
)

set(
    INCLUDES 
    src/
    src/screens
)

# Declaring our executable
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
    -DRESOURCES_DIR=\"${CMAKE_SOURCE_DIR}/resources\"
)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib m)
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    "${PROJECT_SOURCE_DIR}"
)
