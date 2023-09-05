add_executable(process  main.cpp)
add_executable(echoall echoall.cpp)
add_executable(processRelation processRelation.cpp)


install(TARGETS echoall    DESTINATION bin)