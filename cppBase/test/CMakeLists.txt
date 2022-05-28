
add_executable(basic_test basicTest.cpp)
target_link_libraries(basic_test PRIVATE cppBase)

add_executable(inherit_test inheritTest.cpp)
target_link_libraries(inherit_test PRIVATE cppBase)

add_executable(poly_test polyTest.cpp)
target_link_libraries(poly_test PRIVATE cppBase)




