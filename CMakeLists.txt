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

# SDL3

set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

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

# ImGui

FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.9b
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(ImGui)

# CMakeRC script (for resources, the png)

file(DOWNLOAD 
    "https://raw.githubusercontent.com/vector-of-bool/cmrc/master/CMakeRC.cmake" 
    "${CMAKE_BINARY_DIR}/CMakeRC.cmake"
)

include("${CMAKE_BINARY_DIR}/CMakeRC.cmake")

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

# create a resource library named "lugi"
cmrc_add_resource_library(lugi
    NAMESPACE da_luiggi
    "res/luiggi.png"
)

add_executable(Luiggi
    "src/main.cpp"
    "src/new_super_mario_font_u.cpp"
)

target_link_libraries(Luiggi
    PRIVATE
        ImGui
        lugi
)

###################################
#
# Windows specific
#
###################################

if (WIN32)
    # console in debug, no console in release
    set_target_properties(Luiggi PROPERTIES
        WIN32_EXECUTABLE $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:ON>
    )
endif()
