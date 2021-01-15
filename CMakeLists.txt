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
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED)

include_directories(
	dep/argvparse/include
	dep/spdlog/include
	dep/glm/
	dep
	src
)

set(CMAKE_CXX_STANDARD 17)
set(SPDLOG_BUILD_SHARED)
set(CMAKE_CXX_STANDARD_LIBRARIES -lstdc++fs)
add_definitions(
		-DGLM_ENABLE_EXPERIMENTAL
		-DGLFW_INCLUDE_NONE
)

# Third party libraries, compiled with CMAKE
add_subdirectory(dep/argvparse)
add_subdirectory(dep/spdlog)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/util/CLIConfig.cpp
	src/util/Time.cpp
	src/util/Random.cpp
	src/window/LinuxWindow.cpp
	src/events/Event.cpp

	src/ecs/Component.cpp
)

target_link_libraries(${PROJECT_NAME}
	pthread
	glfw GLEW GL
	assimp

	spdlog argvparse
)

target_precompile_headers(${PROJECT_NAME} PUBLIC src/lifepch.h)

install(TARGETS ${PROJECT_NAME})

# =============================== UNIT TESTS ================================= #
enable_testing()
add_subdirectory( test )
