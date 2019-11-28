#----------------------------------------------------------------------------
#   CMakeLists.txt    Defines the build and install configurations for the 
#                     Dragon Recipes project
#             
#   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is distributed under 
#   the MIT License. See LICENSE.TXT for details.
#----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# Specify the minimum version of CMake required to build the project.
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.0)

# ----------------------------------------------------------------------------
# Set the name of the project. ----------------------------------------------------------------------------
set(name "dragonproject")
project(${name})


add_subdirectory(dragonrecipes)
add_subdirectory(knight)

add_dependencies(knight dragonrecipes0)




