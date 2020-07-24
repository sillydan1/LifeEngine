#include <life_core.h>
#include <string>
#include "kuma/shader_compiler/LifeShaderCompiler.hpp"

TEST(ShaderCompilation, FileReading) {
	using namespace life::kuma::shader_compiler;
	auto shadersources = LifeShaderCompiler::Parse("./res/shaders/parsertest.glsl");
	ASSERT_EQ(true, shadersources.used_stages[ShaderType::VERTEX]);
}

TEST(ShaderCompilation, FileInclusion) {
	using namespace life::kuma::shader_compiler;
	auto shadersources = LifeShaderCompiler::Parse("./res/shaders/parsertest.glsl");
	EXPECT_EQ(true, shadersources.used_stages[ShaderType::VERTEX]);
	EXPECT_EQ(true, shadersources.used_stages[ShaderType::FRAGMENT]);
	
	EXPECT_STREQ("Vertex Part\n", shadersources.shaders_stages[ShaderType::VERTEX].c_str());
	EXPECT_STREQ("Fragment Part\n", shadersources.shaders_stages[ShaderType::FRAGMENT].c_str());
}
