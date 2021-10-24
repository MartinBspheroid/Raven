cmake_minimum_required(VERSION 3.0)
project(Raven)
set(CMAKE_CXX_STANDARD 17)

#=== LIBRARY: cimgui + Dear ImGui
add_library(cimgui STATIC 
    cimgui/cimgui.cpp 
    cimgui/cimgui.h 
    cimgui/imgui/imgui.cpp
    cimgui/imgui/imgui.h
    cimgui/imgui/imgui_widgets.cpp 
    cimgui/imgui/imgui_draw.cpp 
    cimgui/imgui/imgui_tables.cpp
    cimgui/imgui/imgui_demo.cpp)
target_include_directories(cimgui INTERFACE cimgui)

#=== LIBRARY: sokol
# add headers to the the file list because they are useful to have in IDEs
set(SOKOL_HEADERS
    sokol/sokol_gfx.h
    sokol/sokol_app.h
    sokol/sokol_time.h
    sokol/sokol_imgui.h
    sokol/sokol_audio.h
    sokol/sokol_glue.h)

add_library(sokol STATIC sokol/sokol.c ${SOKOL_HEADERS})
target_link_libraries(sokol PUBLIC cimgui)
target_include_directories(sokol INTERFACE sokol)

#=== EXECUTABLE: raven
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
add_executable(raven WIN32 main.cpp)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT raven)
else()
add_executable(raven main.cpp)
endif()
target_link_libraries(raven sokol)

# explicitly strip dead code
if (CMAKE_C_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_SYSTEM_NAME STREQUAL Emscripten)
target_link_options(raven PRIVATE LINKER:-dead_strip)
endif()

# this hack removes the xxx-CMakeForceLinker.cxx dummy file
set_target_properties(raven PROPERTIES LINKER_LANGUAGE CXX)
if(MSVC)
target_compile_options(raven PUBLIC "/Zc:__cplusplus")
endif()