cmake_minimum_required(VERSION 3.13)
project(sitemanager)

set(CMAKE_CXX_STANDARD 11)

# Add block directories
aux_source_directory(. SITEMANAGER_SOURCE_FILES)

if(__ANDROID__)
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../android-libs)
else()
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../../libs)
endif()

add_library(sitemg STATIC ${SITEMANAGER_SOURCE_FILES})
