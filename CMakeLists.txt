cmake_minimum_required(VERSION 3.5)

project(top-fifeteen LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets Concurrent Charts REQUIRED)

if(ANDROID)
  add_library(top-fifeteen SHARED
    main.cpp
    plotter.cpp
    plotter.h
    plotter.ui
  )
else()
  add_executable(top-fifeteen
    main.cpp
    threads.h
    threads.cpp
    top-fifteen.cpp
    top-fifteen.h
    plotter.cpp
    plotter.h
    plotter.ui
  )
endif()

target_link_libraries(top-fifeteen PRIVATE Qt5::Widgets Qt5::Concurrent Qt5::Charts)
