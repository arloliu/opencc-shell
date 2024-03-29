cmake_minimum_required(VERSION 2.8.9)
# project settings
set (PROJECT_NAME opencc-shell)
project (${PROJECT_NAME} CXX)

include(${CMAKE_SOURCE_DIR}/cmake/QtWrapper.cmake)

# options
option(WITH_QT5 "Build with QT5" OFF)

# Windows Settings
if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CMAKE_STATIC_LIBRARY_PREFIX ${CMAKE_INSTALL_PREFIX})
endif (WIN32)

# Compiler Flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")
endif()

# Mac OS X settings
set(CMAKE_MACOSX_RPATH 1)

# Directories
set(OPENCC_SRC_DIR ${CMAKE_SOURCE_DIR}/lib/OpenCC/src)
set(UCHARDET_SRC_DIR ${CMAKE_SOURCE_DIR}/lib/uchardet/src)

add_definitions(-DOPENCC_OCD_DIR="${CMAKE_INSTALL_PREFIX}/share/opencc-shell")
# QT settings
if (WITH_QT5)
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5LinguistTools REQUIRED)
    find_package(Qt5Concurrent REQUIRED)
    set(CMAKE_AUTOMOC ON)
else()
    # QT4
    find_package(Qt4 REQUIRED QtCore QtGui)
    include(${QT_USE_FILE})
    set(CMAKE_AUTOMOC ON)

    message("Using Qt Libraries:")
    foreach(lib ${QT_LIBRARIES})
        message("   ${lib}")
    endforeach()
endif()

# Subdirectories
add_subdirectory(lib/OpenCC)
add_subdirectory(lib/uchardet)
add_subdirectory(src)

# Desktop and icon installation
install(FILES opencc-shell.desktop DESTINATION share/applications)
install(FILES src/res/opencc-shell.svg DESTINATION share/pixmaps)
