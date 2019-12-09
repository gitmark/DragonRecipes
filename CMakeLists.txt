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

#execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/copy-version.sh
#WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

execute_process(COMMAND ${CMAKE_SOURCE_DIR}/cxx-version.sh ${CMAKE_CXX_COMPILER}
                OUTPUT_VARIABLE cxxver)
string(STRIP ${cxxver} cxxver)

execute_process(COMMAND ${CMAKE_SOURCE_DIR}/dr-version.sh
	            OUTPUT_VARIABLE drver
				WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
				)

string(STRIP ${drver} drver)

add_subdirectory(DragonRecipes)
add_subdirectory(Knight)
add_subdirectory(Castle)

add_dependencies(knight dragonrecipes${drver}-${cxxver})
add_dependencies(castle dragonrecipes${drver}-${cxxver})
