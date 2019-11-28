cmake_minimum_required(VERSION 3.1.0)

set(name castle)
project(${name})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(Qt5_DIR "~/Qt/5.12.6/clang_64/lib/cmake/Qt5")

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
include_directories(../dragonrecipes/include)
#include_directories(/usr/local/include)
if (CMAKE_GENERATOR MATCHES "Unix Makefiles")
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/dragonrecipes)
#link_directories(/usr/local/lib)
else()
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../build/dragonrecipes)
endif()

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -rpath /Users/mark/Qt/5.12.6/clang_64/lib")

add_executable(castle ${UI_FILES} ${SRC_FILES} ${HEADER_FILES} ${QRC_FILES})

target_link_libraries(castle Qt5::Widgets)

# ----------------------------------------------------------------------------
# Link the app with the library
# ----------------------------------------------------------------------------
if(WIN32)
	target_link_libraries(${name} dragonrecipes0.lib)
else()
	target_link_libraries(${name} dragonrecipes0)
endif()

# ----------------------------------------------------------------------------
# Install the app in the bin directory
# ----------------------------------------------------------------------------
install(TARGETS ${name}
	RUNTIME DESTINATION bin)

