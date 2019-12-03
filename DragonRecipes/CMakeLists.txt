#----------------------------------------------------------------------------
#   CMakeLists.txt    Defines the build and install configurations for the 
#                     Dragon Recipes shared library
#             
#   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
#   distributed under the MIT License. See LICENSE.TXT for details.
#----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# Specify the minimum version of CMake required to build the project.
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.0)

# ----------------------------------------------------------------------------
# Set the name of the project. The major version number is appended to the 
# base name.
# ----------------------------------------------------------------------------
#https://stackoverflow.com/questions/27660048/cmake-check-if-mac-os-x-use-apple-or-apple
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOS TRUE)
endif()

set(maj_ver 0)
set(min_ver 0)
set(patch_ver 0)
set(full_ver ${maj_ver}.${min_ver}.${patch_ver})

set(base_name dragonrecipes)
set(name ${base_name}${maj_ver})
project(${name})
if(WIN32)
set(ext lib)
elseif(MACOS)
set(ext dylib)
else()
set(ext so)
endif()

# ----------------------------------------------------------------------------
# Turn off RPATH for the library install name on macOS.
# ----------------------------------------------------------------------------
set(CMAKE_MACOSX_RPATH OFF)

# ----------------------------------------------------------------------------
# Define DRAGON_RECIPES_LIB_BUILD so that DRAGON_EXPORT will be defined properly 
# for exporting symbols. DRAGON_EXPORT is used in the source code.
# ----------------------------------------------------------------------------
add_definitions("-DDRAGON_RECIPES_LIB_BUILD")

# ----------------------------------------------------------------------------
# Add the include directory to the include search paths.
# ----------------------------------------------------------------------------
include_directories(include)

if(WIN32)

# ----------------------------------------------------------------------------
# /std:c++14    Use the C++ 14 standard.
# ----------------------------------------------------------------------------
    set(CMAKE_CXX_FLAGS "/std:c++14 /EHsc ${CMAKE_CXX_FLAGS}")
else()

# ----------------------------------------------------------------------------
# C++ compiler flags:
# -O0                   Turn off optimizations to allow for easier debugging.
# -std=c++14            Use the C++ 14 standard.
# -Wall                 Enable all warnings.
# -fvisibility=hidden   Hide all symbols not explicitly exported.
# ----------------------------------------------------------------------------
	set(CMAKE_CXX_FLAGS "-O0 -std=c++14 -Wall -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
endif()

# ----------------------------------------------------------------------------
# Set the list of source files.
# ----------------------------------------------------------------------------
FILE (GLOB SRC_FILES src/*.cpp)

# ----------------------------------------------------------------------------
# Set the list of include files.
# ----------------------------------------------------------------------------
FILE (GLOB HEADER_FILES include/*.h include/DragonRecipes/*.h)

# ----------------------------------------------------------------------------
# Set the list of private include files.
# ----------------------------------------------------------------------------
FILE (GLOB PRIVATE_HEADER_FILES include/PrivateHeaders/*.h)

# ----------------------------------------------------------------------------
# Create the shared library.
# ----------------------------------------------------------------------------
add_library(${name} SHARED ${SRC_FILES} ${HEADER_FILES} ${PRIVATE_HEADER_FILES})

# ----------------------------------------------------------------------------
# Set the library's install name directory for macOS.
# ----------------------------------------------------------------------------
set_target_properties(${name}
                      PROPERTIES 
                      INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "default install path" FORCE )
endif()

INSTALL(CODE "execute_process(COMMAND mkdir -p 
${CMAKE_INSTALL_PREFIX}/lib )" )

INSTALL(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
${CMAKE_INSTALL_PREFIX}/${base_name} )" )

INSTALL(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory 
${CMAKE_INSTALL_PREFIX}/${base_name}/lib )" )

INSTALL(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
${CMAKE_INSTALL_PREFIX}/${base_name}/${full_ver}/include 
)" )

INSTALL(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
${CMAKE_INSTALL_PREFIX}/${base_name}/${full_ver}/include/${base_name} 
)" )

# ----------------------------------------------------------------------------
# Set the destination directories for the library's install.
# ----------------------------------------------------------------------------
install(TARGETS ${name} 
   LIBRARY DESTINATION ${base_name}/${full_ver}/lib  # .dylib file is installed here for macOS
                            			     # .so file is installed here for Linux
   ARCHIVE DESTINATION ${base_name}/${full_ver}/lib  # .lib file is installed here for Windows
   RUNTIME DESTINATION ${base_name}/${full_ver}/bin) # .dll file is installed here for Windows

INSTALL (
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${base_name}/${full_ver}/include
    FILES_MATCHING PATTERN "*.h")

INSTALL (
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/${base_name}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${base_name}/${full_ver}/include/${base_name}
    FILES_MATCHING PATTERN "*.h")

INSTALL(CODE "execute_process(COMMAND ln -nsf 
${CMAKE_INSTALL_PREFIX}/${base_name}/${maj_ver}/include/${base_name}
${CMAKE_INSTALL_PREFIX}/include/${base_name} )" )

INSTALL(CODE "execute_process(COMMAND ln -nsf 
${CMAKE_INSTALL_PREFIX}/${base_name}/${maj_ver}/include/DragonRecipes.h
${CMAKE_INSTALL_PREFIX}/include/DragonRecipes.h )" )

INSTALL(CODE "execute_process(COMMAND ln -nsf 
${CMAKE_INSTALL_PREFIX}/${base_name}/${full_ver} 
${CMAKE_INSTALL_PREFIX}/${base_name}/${maj_ver} )" )

INSTALL(CODE "execute_process(COMMAND ln -nsf
${CMAKE_INSTALL_PREFIX}/${base_name}/${maj_ver}/lib/lib${name}.${ext} 
${CMAKE_INSTALL_PREFIX}/${base_name}/lib/lib${name}.${ext} )" )

INSTALL(CODE "execute_process(COMMAND ln -nsf 
${CMAKE_INSTALL_PREFIX}/${base_name}/lib/lib${name}.${ext} 
${CMAKE_INSTALL_PREFIX}/lib/lib${name}.${ext} )" )

if(WIN32)
elseif(MACOS)
else()
INSTALL(CODE "execute_process(COMMAND ldconfig /usr/local/lib)")
endif()


