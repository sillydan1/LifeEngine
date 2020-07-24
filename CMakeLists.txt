cmake_minimum_required(VERSION 3.7)
project(lifeengine)
find_package(glfw3)
find_package(GLEW)
# find_package(bullet) # TODO: Figure out a way that this doesnt end up in a warning
find_package(assimp) # TODO: This issues a warning...

# CFLAGS
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -g -std=c++17 -Wno-int-to-pointer-cast -Wno-sign-compare -DGLM_ENABLE_EXPERIMENTAL -DGLFW_INCLUDE_NONE")
add_definitions(
	-DGLM_ENABLE_EXPERIMENTAL
	-DGLFW_INCLUDE_NONE
	# LIFE Definitions
	-DLIFE_ENABLE_ASSERTS
	-DLIFE_ENABLE_ERR_BREAKPOINTS
	-DLIFE_ENABLE_OGLERR_BREAKPOINTS
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(BDIR "build")
set(SRCDIR "src")
set(OBJDIR "obj")
set(RESDIR "res")
set(DEPDIR "dep")

include(CMakeDefinitions.cmake)

include_directories(
	${SRCDIR}
	${RESDIR}
	${RESDIR}/shaders/  # Used for some shader specifications
	${DEPDIR}
	${DEPDIR}/imgui
    /usr/include/bullet
)

# =============================== CORE LIBS ================================== #
add_library(lifecore
	${SRCDIR}/applications/Application.cpp
	${SRCDIR}/applications/LifeApplication.cpp
	${SRCDIR}/life_core.cpp
	${SRCDIR}/util/Time.cpp
	${SRCDIR}/util/DebugInfo.cpp
	${SRCDIR}/systems/rendering/RenderSystem.cpp
	${SRCDIR}/systems/rendering/AnimationSystem.cpp
	${SRCDIR}/systems/rendering/ParticleSystem.cpp
	${SRCDIR}/systems/DebugSystem.cpp
	${SRCDIR}/events/Event.cpp
	${SRCDIR}/util/Random.cpp
	${SRCDIR}/systems/rendering/GizmosRenderSystem.cpp
	${SRCDIR}/systems/rendering/GizmosRenderSystem.hpp
	${SRCDIR}/util/FileSystem.cpp
)
add_library(lifelayers
	${SRCDIR}/layers/LayerStack.cpp
	${SRCDIR}/layers/EditorLayer.cpp
	${SRCDIR}/layers/RenderLayer.cpp
	${SRCDIR}/layers/SandboxLayer.cpp
	${SRCDIR}/layers/SelectorLayer.cpp
	${SRCDIR}/layers/staging/ComponentStaging.cpp
	${SRCDIR}/layers/PrimitiveConstructor.cpp
)
add_library(lifeeditor
	${SRCDIR}/layers/editors/EditorMenus.cpp
	${SRCDIR}/layers/editors/KumaTypeEditors.cpp
)
add_library(lifeecs
	${SRCDIR}/ecs/ECS.hpp
	${SRCDIR}/ecs/Component.cpp
	${SRCDIR}/ecs/Entity.cpp
)
add_library(kuma
	${SRCDIR}/kuma/utils.cpp
	${SRCDIR}/kuma/IndexBuffer.cpp
	${SRCDIR}/kuma/VertexBuffer.cpp
	${SRCDIR}/kuma/VertexArray.cpp
	${SRCDIR}/kuma/VertexBufferLayout.cpp
	${SRCDIR}/kuma/FrameBuffer.cpp
	${SRCDIR}/kuma/AAFrameBuffer.cpp
	${SRCDIR}/kuma/Texture.cpp
	${SRCDIR}/kuma/CubeTexture.cpp
	${SRCDIR}/kuma/Skybox.cpp
	${SRCDIR}/kuma/Shader.cpp
	${SRCDIR}/kuma/Model.cpp
	${SRCDIR}/kuma/Renderer.cpp
	${SRCDIR}/kuma/Material.cpp 
	${SRCDIR}/kuma/Transform.cpp
	${SRCDIR}/kuma/kuma_math_types.cpp
	${SRCDIR}/kuma/animation/AnimatedModel.cpp
	${SRCDIR}/kuma/default_resources.cpp
	${SRCDIR}/kuma/LifeSelectable.hpp
)
add_library(shadercompiler
	${SRCDIR}/kuma/shader_compiler/Precompiler.cpp
	${SRCDIR}/kuma/shader_compiler/LifeShaderCompiler.cpp
)
add_library(lifecli
	${SRCDIR}/command-line-interface/cli.cpp
)
add_library(lifeimgui
	${DEPDIR}/imgui/imconfig.h
	${DEPDIR}/imgui/imgui.h
	${DEPDIR}/imgui/imgui.cpp
	${DEPDIR}/imgui/imgui_demo.cpp
	${DEPDIR}/imgui/imgui_draw.cpp
	${DEPDIR}/imgui/imgui_internal.h
	${DEPDIR}/imgui/imgui_widgets.cpp
	${DEPDIR}/imgui/imstb_rectpack.h
	${DEPDIR}/imgui/imstb_textedit.h
	${DEPDIR}/imgui/imstb_truetype.h
	# opengl
	${SRCDIR}/imgui/imguibuild.cpp
)
# TODO: This should all be cleaned up at some point
target_include_directories(lifeimgui PRIVATE
	${DEPDIR}/imgui
)
add_library(stbimage
	${SRCDIR}/kuma/stb_image/stb_image.cpp
)
add_library(lifewindowing
	${SRCDIR}/window/LinuxWindow.cpp # TODO: Figure something out with WindowsWindow.cpp
)
add_library(lifeconfig
	${SRCDIR}/config/libconfig.cpp
)

target_link_libraries(lifelayers
	lifecore
	lifeimgui
	lifeecs
	kuma
)

target_link_libraries(lifeeditor
	lifelayers
)

add_library(lifeevents
	${SRCDIR}/events/Event.cpp
)

# =============================== EXECUTABLES ================================ #
add_executable(${PROJECT_NAME}
	${SRCDIR}/main.cpp
)

target_link_libraries(${PROJECT_NAME}
	${THREADING}
	lifeimgui
	lifecore
	lifewindowing
	lifeconfig
	lifeecs
	lifeevents
	lifecli
	lifelayers
	lifeeditor
	kuma
	shadercompiler
	# ------
	stbimage
	glfw
	GLEW
	GL
	assimp
	# ------
	# Bullet Physics
	BulletDynamics
	BulletCollision
	LinearMath
	# ------
	# filesystem
	stdc++fs
)
install(TARGETS ${PROJECT_NAME}
	CONFIGURATIONS Debug
	RUNTIME DESTINATION Debug/bin
)
install(TARGETS ${PROJECT_NAME}
	CONFIGURATIONS Release
	RUNTIME DESTINATION Release/bin
)
# =============================== POST BUILD ================================= #
if(MSVC)
# Windows POST Build stuff
else(MSVC)
# Non-windows POST Build stuff
	add_custom_command(TARGET ${PROJECT_NAME}
		PRE_BUILD
		COMMAND cp -r ${CMAKE_SOURCE_DIR}/${RESDIR} .
#		COMMAND cp .${BDIR}/compile_commands.json .${BDIR}/..
	)
endif(MSVC)
# TODO:We can also do something with compilation of shaders as a POST_BUILD step

# =============================== UNIT TESTS ================================= #
if (MSVC)
	# enable_testing()
	# add_subdirectory( test )
else(MSVC)
	#enable_testing()
	#add_subdirectory( test )
endif(MSVC)


