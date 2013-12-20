cmake_minimum_required(VERSION 2.6)

############################################################################
#
#  The Mouse 1
#
#  Copyright (C) 2010   Ondrej Danek <ondrej.danek@gmail.com>
# 
#############################################################################

# project name
project(Mouse1)

#########################################################################
# compilation details
#########################################################################

# turn off some warnings under msc
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")    
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
endif(WIN32)

# show all warnings under gcc
if(UNIX)
	# gnu compiler
	if(CMAKE_COMPILER_IS_GNUCXX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -mfpmath=sse -msse2")
	endif(CMAKE_COMPILER_IS_GNUCXX)
endif(UNIX)       

#########################################################################
#
# The Mouse 1
#  
#########################################################################

########################
#  Project source files
########################

# set the list of source files
set(M1_SOURCES

    source/draw.cpp
    source/hlavni.cpp
    source/load.cpp
    source/menu.cpp
    source/mouse1.cpp
    source/move.cpp
    source/project.h
    source/setup.cpp

    source/mylib/myerror.cpp
    source/mylib/myfile.cpp
    source/mylib/mykh3.cpp
    source/mylib/mylang.cpp
    source/mylib/mylib.h
    source/mylib/mytools.cpp
    
    source/core/co_core.h
    source/core/co_font.cpp
    source/core/co_fps.cpp
    source/core/co_input.cpp
    source/core/co_main.cpp
    source/core/co_vid.cpp
)

if(WIN32)
    set(M1_SOURCES ${M1_SOURCES} source/mouse1.rc)
endif(WIN32)

########################
#  Add application
########################

set(M1_APP_NAME "mouse1" CACHE STRING "Filename of the application.")
set(M1_APP_DEBUG_NAME "mouse1d" CACHE STRING "Filename of the debug version of the application.")
add_executable(${M1_APP_NAME} ${M1_SOURCES})
set_target_properties(${M1_APP_NAME} PROPERTIES VERSION 1.0.0 DEBUG_OUTPUT_NAME ${M1_APP_DEBUG_NAME})

#########################################################################
# External dependencies
#########################################################################

# OpenGL
if(WIN32)
    target_link_libraries(${M1_APP_NAME} opengl32.lib)
else(WIN32)
    find_library(LIB_OPEN_GL GL DOC "Path to OpenGL import library")
    target_link_libraries(${M1_APP_NAME} ${LIB_OPEN_GL})
endif(WIN32)

# SDL
find_path(HEADERS_SDL SDL/SDL.h DOC "Path to SDL headers")
include_directories(${HEADERS_SDL})
if(WIN32)
    find_library(LIB_SDL SDL DOC "Path to SDL import library (release version)")
    find_library(LIB_SDL_MAIN SDLmain DOC "Path to SDLmain import library (release version)")
    find_library(LIB_SDL_D SDLd DOC "Path to SDL import library (debug version)")
    find_library(LIB_SDL_MAIN_D SDLmaind DOC "Path to SDLmain import library (debug version)")
    target_link_libraries(${M1_APP_NAME} optimized ${LIB_SDL} optimized ${LIB_SDL_MAIN} debug ${LIB_SDL_D} debug ${LIB_SDL_MAIN_D})
else(WIN32)
    find_library(LIB_SDL SDL DOC "Path to SDL import library")
    find_library(LIB_SDL_MAIN SDLmain DOC "Path to SDLmain import library")
    target_link_libraries(${M1_APP_NAME} ${LIB_SDL} ${LIB_SDL_MAIN} ${LIB_SDL_MIXER})
endif(WIN32)

########################
#  Install application
########################

# install binary files
install(TARGETS ${M1_APP_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
