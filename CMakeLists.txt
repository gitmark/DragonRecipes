#----------------------------------------------------------------------------
#   CMakeLists.txt    Defines the build and install configurations for the 
#                     Dragon Recipes project
#             
#   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is 
#   distributed under the MIT License. See LICENSE.TXT for details.
#----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# Specify the minimum version of CMake required to build the project.
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.0)

# ----------------------------------------------------------------------------
# Set the name of the project. 
# ----------------------------------------------------------------------------
set(name "DragonProject")
project(${name})

# ----------------------------------------------------------------------------
# Turn off RPATH for the library install name on macOS.
# ----------------------------------------------------------------------------
set(CMAKE_MACOSX_RPATH OFF)

add_subdirectory(DragonRecipes)
add_subdirectory(Knight)
add_subdirectory(Castle)

add_dependencies(knight dragonrecipes0)
add_dependencies(castle dragonrecipes0)




