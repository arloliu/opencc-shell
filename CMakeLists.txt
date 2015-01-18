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
  add_definitions(
    -std=c++11
    -Wall
  )
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_definitions(
    -std=c++0x
    -Wall
  )
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_definitions(
    /Wall
    /D "_CRT_SECURE_NO_WARNINGS"
  )
endif()

# Mac OS X settings
set(CMAKE_MACOSX_RPATH 1)

# Directories
set(OPENCC_SRC_DIR ${CMAKE_SOURCE_DIR}/lib/OpenCC/src)
set(UCHARDET_SRC_DIR ${CMAKE_SOURCE_DIR}/lib/uchardet/src)

# QT settings
if (WITH_QT5)
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5LinguistTools REQUIRED)
    find_package(Qt5Concurrent REQUIRED)
    set(CMAKE_AUTOMOC ON)
    #~ include_directories(${Qt5Widgets_INCLUDE_DIRS})
    #~ include_directories(${Qt5Concurrent_INCLUDE_DIRS})
    #~ add_definitions(${Qt5Widgets_DEFINITIONS})
    #~ add_definitions(${Qt5Concurrent_DEFINITIONS})
    #~ set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Concurrent_COMPILE_FLAGS}")
    #~ set(QT_LIBRARIES ${Qt5Widgets_LIBRARIES} ${Qt5Concurrent_LIBRARIES})

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

