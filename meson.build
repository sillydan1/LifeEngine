project('lifeengine', 'cpp', default_options : ['cpp_std=c++17'])

compiler=meson.get_compiler('cpp')
add_global_arguments('-Wno-non-virtual-dtor', '-Wno-sign-compare', '-Wno-class-memaccess', '-Wno-int-to-pointer-cast', language : 'cpp')

include=[
	include_directories('src'),
	include_directories('res'),
	include_directories('res/shaders'),
	include_directories('dep'),
	include_directories('dep/imgui'),
	include_directories('/usr/include/bullet')
]

ext_depends=[
	dependency('glm'),
	dependency('bullet'),
	dependency('glew'),
	dependency('glfw3'),
	dependency('assimp'),
	dependency('threads')
]
# Life Libs
core=[
	'src/applications/Application.cpp',
	'src/applications/LifeApplication.cpp',
	'src/life_core.cpp',
	'src/util/Time.cpp',
	'src/util/DebugInfo.cpp',
	'src/systems/rendering/RenderSystem.cpp',
	'src/systems/rendering/AnimationSystem.cpp',
	'src/systems/rendering/ParticleSystem.cpp',
	'src/systems/DebugSystem.cpp',
	'src/events/Event.cpp',
	'src/util/Random.cpp',
	'src/util/FileSystem.cpp',
	'src/systems/rendering/GizmosRenderSystem.cpp',
]
layers=[
	'src/layers/LayerStack.cpp',
	'src/layers/EditorLayer.cpp',
	'src/layers/RenderLayer.cpp',
	'src/layers/SandboxLayer.cpp',
	'src/layers/SelectorLayer.cpp',
	'src/layers/staging/ComponentStaging.cpp',
	'src/layers/PrimitiveConstructor.cpp',
]
editor=[
	'src/layers/editors/EditorMenus.cpp',
	'src/layers/editors/KumaTypeEditors.cpp',
]

ecs=[
	'src/ecs/ECS.hpp',
	'src/ecs/Component.cpp',
	'src/ecs/Entity.cpp',
]

kuma=[ # TODO: Rename kuma to something less stupid
	'src/kuma/utils.cpp',
	'src/kuma/IndexBuffer.cpp',
	'src/kuma/VertexBuffer.cpp',
	'src/kuma/VertexArray.cpp',
	'src/kuma/VertexBufferLayout.cpp',
	'src/kuma/FrameBuffer.cpp',
	'src/kuma/AAFrameBuffer.cpp',
	'src/kuma/Texture.cpp',
	'src/kuma/CubeTexture.cpp',
	'src/kuma/Skybox.cpp',
	'src/kuma/Shader.cpp',
	'src/kuma/Model.cpp',
	'src/kuma/Renderer.cpp',
	'src/kuma/Material.cpp',
	'src/kuma/Transform.cpp',
	'src/kuma/kuma_math_types.cpp',
	'src/kuma/animation/AnimatedModel.cpp',
	'src/kuma/default_resources.cpp',
	'src/kuma/shader_compiler/Precompiler.cpp',
	'src/kuma/shader_compiler/LifeShaderCompiler.cpp',
]

cli=[
	'src/command-line-interface/cli.cpp',
]

lifeimgui=[
	'dep/imgui/imconfig.h',
	'dep/imgui/imgui.h',
	'dep/imgui/imgui.cpp',
	'dep/imgui/imgui_demo.cpp',
	'dep/imgui/imgui_draw.cpp',
	'dep/imgui/imgui_internal.h',
	'dep/imgui/imgui_widgets.cpp',
	'dep/imgui/imstb_rectpack.h',
	'dep/imgui/imstb_textedit.h',
	'dep/imgui/imstb_truetype.h',
	# opengl
	'src/imgui/imguibuild.cpp',
]

stbimage=[
	'src/kuma/stb_image/stb_image.cpp',
]

lifewindowing=[
	'src/window/LinuxWindow.cpp',
]

libconfig=[
	'src/config/libconfig.cpp',
]

events=[
	'src/events/Event.cpp',
]

# This copies the resources folder into the build directory
resourcesfolder=custom_target('cpytarget',
	input: ['res'],
	output: ['res'],
	command: ['cp','-r','@INPUT@','@OUTDIR@'],
	build_by_default: true
)
# This copies the reosources folder into the install directory.
install_subdir('res', install_dir : 'bin')

engine=executable('lifeengine',
	['src/main.cpp',
	events,libconfig,lifewindowing,stbimage,lifeimgui,kuma,ecs,layers,core,editor],
	install: true,
	dependencies: [ext_depends],
	include_directories: include
)

