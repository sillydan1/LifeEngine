cmake_minimum_required(VERSION 3.0)
project(life_engine)
# Required packages
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Unit Tests for CORE modules ( This might change over time )
add_executable( 		run_unittests main.cpp )
target_link_libraries(	run_unittests 
	lifecore
	lifewindowing
	lifeconfig
    lifecli
	kuma
	shadercompiler
	${GTEST_LIBRARIES} 
	pthread )

