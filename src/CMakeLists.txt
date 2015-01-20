set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Disable debug output
if (CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DQT_NO_DEBUG_OUTPUT=1)
    if (WIN32)
        set(OPENCC_SHELL_BIN_OPTIONS WIN32)
    endif()
endif()

# set windows icon
if (WIN32)
    set(OPENCC_SHELL_SRCS ${OPENCC_SHELL_SRCS} App.rc)
    add_definitions(-DOpencc_BUILT_AS_STATIC)
endif ()

set(OPENCC_SHELL_SRCS
    main.cpp
    CharsetDetector.cpp
    TextLoader.cpp
    TextConverter.cpp
    MainWindow.cpp
    ConvertSettingWidget.cpp
    FileListWidget.cpp
    SingleConvertWidget.cpp
    BatchConvertWidget.cpp
)

set(OPENCC_SHELL_LIBS
    libuchardet_static
    libopencc
    ${QT_LIBRARIES}
)

include_directories(${OPENCC_SRC_DIR} ${UCHARDET_SRC_DIR})


# Embed translataions into binary
file (GLOB TRANSLATIONS_FILES ../l10n/*.ts)
qt_add_translation_wrapper(QM_FILES ${TRANSLATIONS_FILES})

set(OPENCC_SHELL_QRC_FILES App.qrc)

set(TRANSLATIONS_QRC "${CMAKE_CURRENT_BINARY_DIR}/ts.qrc")
file(WRITE ${TRANSLATIONS_QRC} "<RCC>\n\t<qresource prefix=\"/translations\">")
foreach(QM_FILE ${QM_FILES})
    get_filename_component(QM_FILE_NAME ${QM_FILE} NAME)
    file(APPEND ${TRANSLATIONS_QRC} "\n\t\t<file alias=\"${QM_FILE_NAME}\">${QM_FILE_NAME}</file>")
endforeach()
file(APPEND ${TRANSLATIONS_QRC} "\n\t</qresource>\n</RCC>")
list(APPEND OPENCC_SHELL_QRC_FILES ${TRANSLATIONS_QRC})

qt_add_resources_wrapper(OPENCC_SHELL_QRC_SRCS ${OPENCC_SHELL_QRC_FILES})

# Add ececutable target
add_executable(${PROJECT_NAME}
    ${OPENCC_SHELL_BIN_OPTIONS}
    ${OPENCC_SHELL_SRCS}
    ${OPENCC_SHELL_QRC_SRCS}
)

if (WITH_QT5)
    qt5_use_modules(${PROJECT_NAME} Widgets Concurrent)
endif()

# link against required libraries
target_link_libraries(${PROJECT_NAME} ${OPENCC_SHELL_LIBS})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# copy compiled dictonary files to build dir.
set(OPENCC_DATA_DIR ${CMAKE_CURRENT_BINARY_DIR}/../lib/OpenCC/data)
file(GLOB dictFiles ${OPENCC_DATA_DIR}/*.ocd)
foreach(dictFile ${dictFiles})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E
        copy ${dictFile} ${CMAKE_CURRENT_BINARY_DIR}
    )
    file(RELATIVE_PATH dictName ${OPENCC_DATA_DIR} ${dictFile})
    message("Install ${dictFile} to ${CMAKE_CURRENT_BINARY_DIR}")
endforeach()
