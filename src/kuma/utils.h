#ifndef kuma_STANDARD_H
#define kuma_STANDARD_H
#include <GL/glew.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "openglmath"
#include <util/FileSystem.hpp>

#define SHADERDIR getrespath()+"/shaders"

// LIFE_SHADER_ADDITION_DEFINITIONS
#ifndef LIGHT_DEFINITION_PATH
	#define LIGHT_DEFINITION_PATH getrespath()+"/LIGHT_DEFINITIONS"
#endif

#define ASSERT(x) if(!(x)) assert(x);
#define GLCall(x) GLClearError();\
x;\
assert(GLCheckError(#x))

bool GLCheckError(const char* func_name);
void GLClearError();
bool Vec3NumericDiff(const glm::vec3& a, const glm::vec3& b, float max_diff);

#endif // kuma_STANDARD_H
