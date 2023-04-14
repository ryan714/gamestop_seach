cmake_minimum_required(VERSION 3.5)

project(qtProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(Qt6Network REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core)

#libxml++
include_directories("/usr/include/libxml++-5.0/")
find_package (PkgConfig REQUIRED)
find_package (LibXml2 REQUIRED)
pkg_check_modules (LIBXMLXX REQUIRED IMPORTED_TARGET libxml++-5.0)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        downloader.cpp
        mainwindow.h
        downloader.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(qtProject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qtProject APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qtProject SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qtProject
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(qtProject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Network Qt6::Core)
target_link_libraries(qtProject PRIVATE PkgConfig::LIBXMLXX)


set_target_properties(qtProject PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS qtProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qtProject)
endif()
