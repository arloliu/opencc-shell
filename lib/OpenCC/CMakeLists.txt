#
# Open Chinese Convert
#
# Copyright 2010-2014 BYVoid <byvoid@byvoid.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

######## Project settings
cmake_minimum_required(VERSION 2.8)
set (PACKAGE_NAME opencc)
project (${PACKAGE_NAME} CXX)
include (CTest)
enable_testing()

######## Package information
set (PACKAGE_URL https://github.com/BYVoid/Opencc)
set (PACKAGE_BUGREPORT https://github.com/BYVoid/Opencc/issues)
set (OPENCC_VERSION_MAJOR 1)
set (OPENCC_VERSION_MINOR 0)
set (OPENCC_VERSION_REVISION 2)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set (version_suffix .Debug)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

set (
  OPENCC_VERSION
  ${OPENCC_VERSION_MAJOR}.${OPENCC_VERSION_MINOR}.${OPENCC_VERSION_REVISION}${version_suffix}
)

set(CPACK_SOURCE_PACKAGE_FILE_NAME
  "${PACKAGE_NAME}-${OPENCC_VERSION_MAJOR}.${OPENCC_VERSION_MINOR}.${OPENCC_VERSION_REVISION}"
)
set(CPACK_SOURCE_IGNORE_FILES
  "/build/;/test/dict.ocd;/test/dict.txt;/test/dict.bin;/other/;/opencc.xcodeproj/;/.git/;.gitignore;~$;.pyc;${CPACK_SOURCE_IGNORE_FILES}"
)
include(CPack)

######## Options
option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)

######## Windows

if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CMAKE_STATIC_LIBRARY_PREFIX ${CMAKE_INSTALL_PREFIX})
endif (WIN32)

######## Mac OS X

set(CMAKE_MACOSX_RPATH 1)

######## Directory

set (DIR_PREFIX ${CMAKE_INSTALL_PREFIX})
set (DIR_INCLUDE ${DIR_PREFIX}/include/)
set (DIR_SHARE ${DIR_PREFIX}/share/)
set (DIR_ETC ${DIR_PREFIX}/etc/)
set (DIR_LIBRARY ${DIR_PREFIX}/lib/)

if (DEFINED SHARE_INSTALL_PREFIX)
  set (DIR_SHARE ${SHARE_INSTALL_PREFIX})
endif (DEFINED SHARE_INSTALL_PREFIX)

if (DEFINED INCLUDE_INSTALL_DIR)
  set (DIR_INCLUDE ${INCLUDE_INSTALL_DIR})
endif (DEFINED INCLUDE_INSTALL_DIR)

if (DEFINED SYSCONF_INSTALL_DIR)
  set (DIR_ETC ${SYSCONF_INSTALL_DIR})
endif (DEFINED SYSCONF_INSTALL_DIR)

set (DIR_SHARE_OPENCC ${DIR_SHARE}opencc-shell/)
set (DIR_SHARE_LOCALE ${DIR_SHARE}locale/)

######## Compiler flags

add_definitions(
  -DPKGDATADIR="${DIR_SHARE_OPENCC}"
  -DLOCALEDIR="${DIR_SHARE_LOCALE}"
  -DVERSION="${OPENCC_VERSION}"
  -DPACKAGE_NAME="${PACKAGE_NAME}"
)

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

######## Subdirectories

add_subdirectory(src)
#~ add_subdirectory(doc)
add_subdirectory(data)
#~ add_subdirectory(test)
