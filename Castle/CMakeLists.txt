#----------------------------------------------------------------------------
#   CMakeLists.txt    Defines the build and install configurations for the
#                     Dragon Recipes project
#
#   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
#   distributed under the MIT License. See LICENSE.TXT for details.
#----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.1.0)

execute_process(COMMAND ${CMAKE_SOURCE_DIR}/cxx-version.sh ${CMAKE_CXX_COMPILER}
                OUTPUT_VARIABLE cxxver)
string(STRIP ${cxxver} cxxver)

set(name castle)
project(${name})

#https://stackoverflow.com/questions/27660048/cmake-check-if-mac-os-x-use-apple-or-apple
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOS TRUE)
endif()

if(WIN32)
set(comp msvc2017)
set(basedir "C:")
elseif(MACOS)
set(comp clang)
set(basedir "$ENV{HOME}")
else()
set(comp gcc)
set(basedir "$ENV{HOME}")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(Qt5_DIR "${basedir}/Qt/5.12.6/${comp}_64/lib/cmake/Qt5")

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# ----------------------------------------------------------------------------
# Set the list of source files.
# ----------------------------------------------------------------------------
FILE (GLOB SRC_FILES *.cpp)

# ----------------------------------------------------------------------------
# Set the list of include files.
# ----------------------------------------------------------------------------
FILE (GLOB HEADER_FILES *.h)

# ----------------------------------------------------------------------------
# Set the list of UI files.
# ----------------------------------------------------------------------------
FILE (GLOB UI_FILES *.ui)

# ----------------------------------------------------------------------------
# Set the list of resource files.
# ----------------------------------------------------------------------------
FILE (GLOB QRC_FILES *.qrc)

# ----------------------------------------------------------------------------
# Set the include and link search directories
# ----------------------------------------------------------------------------
include_directories(../DragonRecipes/include .)
#include_directories(/usr/local/include)
if (CMAKE_GENERATOR MATCHES "Unix Makefiles")
link_directories(${CMAKE_BINARY_DIR}/DragonRecipes)
#link_directories(/usr/local/lib)
else()
link_directories(${CMAKE_BINARY_DIR}/DragonRecipes)
endif()

if(WIN32)
    set(CMAKE_CXX_FLAGS "/std:c++14 /EHsc ${CMAKE_CXX_FLAGS}")
elseif(MACOS)
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -rpath $ENV{HOME}/Qt/5.12.6/${comp}_64/lib")
else()
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wl,-rpath,$ENV{HOME}/Qt/5.12.6/${comp}_64/lib")
endif()

add_executable(castle ${UI_FILES} ${SRC_FILES} ${HEADER_FILES} ${QRC_FILES})

set_target_properties( castle
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Castle.app/Contents/MacOS"
)

target_link_libraries(castle Qt5::Widgets)

if(MACOS)
add_custom_command(TARGET ${name}
PRE_BUILD
COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/pre-build.sh
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET ${name}
POST_BUILD
COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/make-bundle.sh
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
${drver}-${cxxver}
)
endif()

# ----------------------------------------------------------------------------
# Link the app with the library
# ----------------------------------------------------------------------------
if(WIN32)
	target_link_libraries(${name} dragonrecipes${drver}-${cxxver}.lib)
else()
	target_link_libraries(${name} dragonrecipes${drver}-${cxxver})
endif()

# ----------------------------------------------------------------------------
# Install the app in the bin directory
# ----------------------------------------------------------------------------
#install(TARGETS ${name}
#	RUNTIME DESTINATION bin)

INSTALL(CODE "execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/uninstall-castle.sh)" )

INSTALL(CODE "execute_process(COMMAND cp -a
${CMAKE_CURRENT_BINARY_DIR}/Castle.app
/Applications/)" )

INSTALL (
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/castle.sh
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	DESTINATION /Applications/Castle.app/Contents/MacOS/)

INSTALL(CODE "execute_process(COMMAND ln -nsf
/Applications/Castle.app/Contents/MacOS/castle.sh
${CMAKE_INSTALL_PREFIX}/bin/castle )" )
