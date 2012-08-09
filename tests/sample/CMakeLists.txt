cmake_minimum_required(VERSION 2.8)

set(PROJECT sample)

project(${PROJECT} CXX)

set(SRC SampleTest.cpp) # + project sources

find_library(CPPUNIT_LIB cppunit)
add_executable(${PROJECT} ${SRC})
target_link_libraries(${PROJECT} ${CPPUNIT_LIB})

install(TARGETS ${PROJECT} DESTINATION tests)
