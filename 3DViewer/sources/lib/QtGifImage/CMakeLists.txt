cmake_minimum_required(VERSION 3.5)

project(GIFLIB VERSION 0.1 LANGUAGES C CXX)

# Set build type if not set
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

# Set compiler flags for Debug build
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g" )

add_library(GIFLIB
    giflib/dgif_lib.c
    giflib/egif_lib.c
    giflib/gif_err.c
    giflib/gif_hash.c
    giflib/gifalloc.c
    giflib/quantize.c
    giflib/gif_hash.h
    giflib/gif_lib_private.h
    giflib/gif_lib.h
    gifimage/qgifglobal.h
    gifimage/qgifimage_p.h
    gifimage/qgifimage.cpp
    gifimage/qgifimage.h
)

target_link_libraries(GIFLIB Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)
