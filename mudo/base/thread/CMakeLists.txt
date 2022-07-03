FILE(GLOB src "*.cpp")
add_library(thread STATIC ${src})
target_include_directories(thread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(thread PUBLIC common)