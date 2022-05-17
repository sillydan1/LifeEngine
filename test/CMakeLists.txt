cmake_minimum_required(VERSION 3.0)
project(lifeengine_tests)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(run_unittests main.cpp)
target_link_libraries(run_unittests
	${GTEST_LIBRARIES} pthread
	di ecs
)
