# Copyright (C) 2020 Asger Gitz-Johansen
# This file is part of life engine.
#
#  lifeengine is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  lifeengine is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with lifeengine.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.7)
project(lifeengine)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(OpenGL REQUIRED)

link_directories(${ASSIMP_LIBRARY_DIRS})
include_directories(
	${GLM_INCLUDE_DIRS}/../include # MacOS is weird
	${GLM_INCLUDE_DIRS}
	${GLFW3_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${ASSIMP_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	dep/argvparse/include
	dep/spdlog/include
	dep/glm/
	dep
	src
)

set(CMAKE_CXX_STANDARD 20)
set(SPDLOG_BUILD_SHARED)
add_definitions(
		-DGLM_ENABLE_EXPERIMENTAL
		-DGLFW_INCLUDE_NONE
)

# Third party libraries, compiled with CMAKE
add_subdirectory(dep/argvparse)
add_subdirectory(dep/spdlog)

# 1st party libraries, compiled with CMAKE
add_subdirectory(src/di)
add_subdirectory(src/ecs)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/util/CLIConfig.cpp
	src/util/Time.cpp
	src/util/Random.cpp
	src/window/LinuxWindow.cpp
	src/events/Event.cpp

	src/engine/Application.cpp
	src/SANDBOX/Sandbox.cpp
	src/SANDBOX/SandboxLayer.cpp

	src/layers/LayerCollection.cpp

	src/rendering/shader/ShaderParser.cpp
	src/rendering/shader/Shader.cpp
	src/extensions/stringextensions.cpp
	src/rendering/basics/Vertex.cpp

	src/rendering/debugging.cpp
)

target_link_libraries(${PROJECT_NAME}
	pthread
	${GLFW_LIBRARIES}
	${OPENGL_LIBRARIES}
	${ASSIMP_LIBRARIES}
	glfw
	GLEW::GLEW

	spdlog argvparse
	di
	ecs
)

target_precompile_headers(${PROJECT_NAME} PUBLIC src/lifepch.h)
target_precompile_headers(${PROJECT_NAME} PUBLIC src/glpch.h)

install(TARGETS ${PROJECT_NAME})

# =============================== UNIT TESTS ================================= #
enable_testing()
add_subdirectory( test )
