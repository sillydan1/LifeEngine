# Copyright (C) 2020 Asger Gitz-Johansen
# This file is part of life engine.
#
#  aaltitoad is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  aaltitoad is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with aaltitoad.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.7)
project(lifeengine)

find_package(glfw3)
find_package(GLEW)
find_package(assimp)

set(CMAKE_CXX_STANDARD 17)
add_definitions(
	-DGLM_ENABLE_EXPERIMENTAL
	-DGLFW_INCLUDE_NONE
)
include_directories(
	src
	dep
)

add_executable(${PROJECT_NAME}
	src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
	pthread
	glfw GLEW GL
	assimp
	stdc++fs # filesystem
)

# =============================== UNIT TESTS ================================= #
enable_testing()
add_subdirectory( test )
