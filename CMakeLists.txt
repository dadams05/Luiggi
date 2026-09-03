cmake_minimum_required(VERSION 3.21)
project(Luiggi LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#####################################################
#
# Download dependencies
#
#####################################################

include(FetchContent)

# ---------------------------------------------------
# SDL3
# ---------------------------------------------------

# set(SDL_SHARED OFF CACHE BOOL "" FORCE)
# set(SDL_STATIC ON  CACHE BOOL "" FORCE)
# set(SDL_INSTALL ON CACHE BOOL "" FORCE)

set(SDL_AUDIO OFF CACHE BOOL "" FORCE)
set(SDL_GPU OFF CACHE BOOL "" FORCE)
set(SDL_CAMERA OFF CACHE BOOL "" FORCE)
set(SDL_JOYSTICK OFF CACHE BOOL "" FORCE)
set(SDL_HAPTIC OFF CACHE BOOL "" FORCE)
set(SDL_HIDAPI OFF CACHE BOOL "" FORCE)
set(SDL_POWER OFF CACHE BOOL "" FORCE)
set(SDL_SENSOR OFF CACHE BOOL "" FORCE)
set(SDL_DIALOG OFF CACHE BOOL "" FORCE)

set(SDL_DUMMYVIDEO OFF CACHE BOOL "" FORCE)
set(SDL_DUMMYAUDIO OFF CACHE BOOL "" FORCE)
set(SDL_DUMMYCAMERA OFF CACHE BOOL "" FORCE)
set(SDL_OFFSCREEN OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.16
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL3)

# Ensure all target variants exist for downstream consumers like SDL_image
# if(TARGET SDL3::SDL3-static)
#     if(NOT TARGET SDL3::SDL3)
#         add_library(SDL3::SDL3 ALIAS SDL3::SDL3-static)
#     endif()
#     if(NOT TARGET SDL3::SDL3-shared)
#         add_library(SDL3::SDL3-shared ALIAS SDL3::SDL3-static)
#     endif()
# endif()

# ---------------------------------------------------
# Intercept SDL_image's find_package(SDL3) call
# ---------------------------------------------------
# file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/cmake/FindSDL3.cmake" "
#     set(SDL3_FOUND TRUE)
#     if(TARGET SDL3::SDL3-static AND NOT TARGET SDL3::SDL3)
#         add_library(SDL3::SDL3 ALIAS SDL3::SDL3-static)
#     endif()
# ")
# list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}/cmake")

# ---------------------------------------------------
# SDL_image
# ---------------------------------------------------

# set(SDLIMAGE_VENDORED ON CACHE BOOL "" FORCE)
# set(SDLIMAGE_DEPS_SHARED OFF CACHE BOOL "" FORCE)
# set(SDLIMAGE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SDL_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.4.4
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SDL_image)

# ---------------------------------------------------
# ImGui
# ---------------------------------------------------

FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.9b
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(ImGui)

#####################################################
#
# Build the ImGui library (depends on SDL3)
#
#####################################################

add_library(ImGui 
    STATIC
        "${imgui_SOURCE_DIR}/imgui.cpp"
        "${imgui_SOURCE_DIR}/imgui_draw.cpp"
        "${imgui_SOURCE_DIR}/imgui_tables.cpp"
        "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp"
)

target_include_directories(ImGui
    PUBLIC
        "${imgui_SOURCE_DIR}"
        "${imgui_SOURCE_DIR}/backends"
)

target_link_libraries(ImGui
    PUBLIC
        SDL3::SDL3
)

###################################
#
# Build main executable
#
###################################

add_executable(Luiggi
    "src/main.cpp"
)

target_link_libraries(Luiggi
    PRIVATE
        ImGui
        SDL3_image::SDL3_image
)

# post-build commands
# add_custom_command(TARGET Luiggi POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "$<TARGET_RUNTIME_DLLS:Luiggi>"
#        "$<TARGET_FILE_DIR:Luiggi>"
#     COMMAND_EXPAND_LISTS
# )
