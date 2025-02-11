add_library(http STATIC httplib.cc)
message(${CMAKE_CURRENT_SOURCE_DIR})
target_link_directories(http PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})