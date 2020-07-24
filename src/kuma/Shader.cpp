#include "Shader.hpp"
#include "utils.h"
#include <unordered_map>
#include <util/overload>
#include <life_core.h>
#include "shader_compiler/ShaderSource.hpp"

namespace life::kuma {
	
	Shader::Shader() 
		: m_filepath(SHADERDIR "/default.shader")
	{
		
	}
	
	Shader::Shader(const std::string& file_path)
		: m_filepath(file_path)
	{
		
	}
	
	Shader::~Shader() {
	    if(m_programID != 0) {
            GLCall(glDeleteProgram(m_programID));
        }
	}
	
	void Shader::Bind() const {
		GLCall(glUseProgram(m_programID));
	}
	
	void Shader::Unbind() const {
		GLCall(glUseProgram(0));
	}
	
	void Shader::Compile(const shader_compiler::ShaderSource& sources) {
		m_programID = CompileShaders(sources);
	}

	void Shader::SetUniform(GLint location, const KumaTypes& param) const {
	    if(location == -1) return;
		std::visit(
			overload(
				[location](const TYPE_PTR(Transform) &v)						        { GLCall(glUniformMatrix4fv(location, 1, false, &v->GetModelMatrix()[0][0])); },
				[location](const TYPE_PTR(Texture) &v)                                  { v->Bind(0); GLCall(glUniform1i(location, v->GetTextureID())); },
				[location](const TYPE_PTR(CubeTexture) &v)                              { v->Bind(0); GLCall(glUniform1i(location, v->GetTextureID())); },
				[location](const TYPE_PTR(Skeleton) &v)                                 {
				    for(auto& b : *v) {
				        if(b.second.bone_index >= 0) {
							GLCall(glUniformMatrix4fv(location + b.second.bone_index, 1, false, &b.second.FinalTransform[0][0]));
						}
                    }
				},

				[location](const TYPE_PTR(glm::bvec2) &v)								{ GLCall(glUniform2iv (location, 1, reinterpret_cast<const GLint*>(&(*v)[0])) ); },
				[location](const TYPE_PTR(glm::bvec3) &v)								{ GLCall(glUniform3iv (location, 1, reinterpret_cast<const GLint*>(&(*v)[0])) ); },
				[location](const TYPE_PTR(glm::bvec4) &v)								{ GLCall(glUniform4iv (location, 1, reinterpret_cast<const GLint*>(&(*v)[0])) ); },
				[location](const TYPE_PTR(glm::ivec2) &v)								{ GLCall(glUniform2iv (location, 1, &(*v)[0]) ); },
				[location](const TYPE_PTR(glm::ivec3) &v)								{ GLCall(glUniform3iv (location, 1, &(*v)[0]) ); },
				[location](const TYPE_PTR(glm::ivec4) &v)								{ GLCall(glUniform4iv (location, 1, &(*v)[0]) ); },
				[location](const TYPE_PTR(glm::uvec2) &v)								{ GLCall(glUniform2uiv(location, 1, &(*v)[0]) ); },
				[location](const TYPE_PTR(glm::uvec3) &v)								{ GLCall(glUniform3uiv(location, 1, &(*v)[0]) ); },
				[location](const TYPE_PTR(glm::uvec4) &v)								{ GLCall(glUniform4uiv(location, 1, &(*v)[0]) ); },
				[location](const TYPE_PTR(glm::vec2)  &v)								{ GLCall(glUniform2fv (location, 1, &(*v)[0]) ); },
				[location](const TYPE_PTR(glm::vec3)  &v)								{ GLCall(glUniform3fv (location, 1, &(*v)[0]) ); },
				[location](const TYPE_PTR(glm::vec4)  &v)								{ GLCall(glUniform4fv (location, 1, &(*v)[0]) ); },
				
				[location](const TYPE_PTR(bool) &v)										{ GLCall(glUniform1i(location, *v)); },

				[location](const TYPE_PTR(std::tuple<float>) &v)						{ GLCall(glUniform1f (location, std::get<0>(*v))); },
				[location](const TYPE_PTR(std::tuple<float, float>) &v)					{ GLCall(glUniform2f (location, std::get<0>(*v), std::get<1>(*v))); },
				[location](const TYPE_PTR(std::tuple<float, float, float>) &v)			{ GLCall(glUniform3f (location, std::get<0>(*v), std::get<1>(*v), std::get<2>(*v))); },
				[location](const TYPE_PTR(std::tuple<float, float, float, float>) &v)	{ GLCall(glUniform4f (location, std::get<0>(*v), std::get<1>(*v), std::get<2>(*v), std::get<3>(*v))); },
				[location](const TYPE_PTR(std::tuple<int>) &v)							{ GLCall(glUniform1i (location, std::get<0>(*v))); },
				[location](const TYPE_PTR(std::tuple<int, int>) &v)						{ GLCall(glUniform2i (location, std::get<0>(*v), std::get<1>(*v))); },
				[location](const TYPE_PTR(std::tuple<int, int, int>) &v)				{ GLCall(glUniform3i (location, std::get<0>(*v), std::get<1>(*v), std::get<2>(*v))); },
				[location](const TYPE_PTR(std::tuple<int, int, int, int>) &v)			{ GLCall(glUniform4i (location, std::get<0>(*v), std::get<1>(*v), std::get<2>(*v), std::get<3>(*v))); },
				[location](const TYPE_PTR(std::tuple<u_int>) &v)						{ GLCall(glUniform1ui(location, std::get<0>(*v))); },
				[location](const TYPE_PTR(std::tuple<u_int, u_int>) &v)					{ GLCall(glUniform2ui(location, std::get<0>(*v), std::get<1>(*v))); },
				[location](const TYPE_PTR(std::tuple<u_int, u_int, u_int>) &v)			{ GLCall(glUniform3ui(location, std::get<0>(*v), std::get<1>(*v), std::get<2>(*v))); },
				[location](const TYPE_PTR(std::tuple<u_int, u_int, u_int, u_int>) &v)	{ GLCall(glUniform4ui(location, std::get<0>(*v), std::get<1>(*v), std::get<2>(*v), std::get<3>(*v))); },

				[location](const TYPE_PTR(glm::mat2) &v)								{ GLCall(glUniformMatrix2fv	 (location, 1, false, &(*v)[0][0])); },
				[location](const TYPE_PTR(glm::mat2x3) &v)								{ GLCall(glUniformMatrix2x3fv(location, 1, false, &(*v)[0][0])); },
				[location](const TYPE_PTR(glm::mat2x4) &v)								{ GLCall(glUniformMatrix2x4fv(location, 1, false, &(*v)[0][0])); },
				[location](const TYPE_PTR(glm::mat3) &v)								{ GLCall(glUniformMatrix3fv	 (location, 1, false, &(*v)[0][0])); },
				[location](const TYPE_PTR(glm::mat3x2) &v)								{ GLCall(glUniformMatrix3x2fv(location, 1, false, &(*v)[0][0])); },
				[location](const TYPE_PTR(glm::mat3x4) &v)								{ GLCall(glUniformMatrix3x4fv(location, 1, false, &(*v)[0][0])); },
				[location](const TYPE_PTR(glm::mat4) &v)								{ GLCall(glUniformMatrix4fv	 (location, 1, false, &(*v)[0][0])); },
				[location](const TYPE_PTR(glm::mat4x2) &v)								{ GLCall(glUniformMatrix4x2fv(location, 1, false, &(*v)[0][0])); },
				[location](const TYPE_PTR(glm::mat4x3) &v)								{ GLCall(glUniformMatrix4x3fv(location, 1, false, &(*v)[0][0])); },

				[location](const float &v)												{ GLCall(glUniform1f (location, v)); },
				[location](const int &v)												{ GLCall(glUniform1i (location, v)); },
				[location](const u_int &v)												{ GLCall(glUniform1i (location, v)); },
				[location](const bool &v)												{ GLCall(glUniform1i (location, v)); },

				[](auto&& a) {
					LIFE_ERR("Could not recognize given type for shader! - Or the type is not supported yet! - %s", typeid(decltype(a)).name());
				}
			), param);
	}
	
	GLint Shader::GetUniformLocation(const std::string& name) const {
		GLCall(GLint location = glGetUniformLocation(m_programID, name.c_str()));
		if(location == -1) {
			// LIFE_WARN("Uniform '%s' does not exist on shader '%s'", name.c_str(), m_filepath.c_str());
		}
		return location;
	}
	
	GLuint CompileShaderStage(const char* src, GLuint stage, const std::string& stage_str) {
		// Create & upload the shader
		GLuint shader_stage = glCreateShader(stage);
		GLCall(glShaderSource(shader_stage, 1, &src, NULL));
		// Compile the shader.
		GLCall(glCompileShader(shader_stage));
		// Compilation Status
		GLint stage_compile_status;
		GLCall(glGetShaderiv(shader_stage, GL_COMPILE_STATUS, &stage_compile_status));
		if (stage_compile_status != GL_TRUE) {
			GLsizei log_length = 0;
			GLchar message[1024];
			GLCall(glGetShaderInfoLog(shader_stage, 1024, &log_length, message));
			LIFE_ERR("Shader Compilation Error: %s \n\t%s", stage_str.c_str(), message);
			
		}
		return shader_stage;
	}
	
	GLuint CompileShaders(const shader_compiler::ShaderSource& shadersource) {
		GLuint program = glCreateProgram();
		// Vertex
		GLuint vert = CompileShaderStage(shadersource.shaders_stages[shader_compiler::ShaderType::VERTEX  ].c_str(), GL_VERTEX_SHADER, "VERTEX");
		// Fragment
		GLuint frag = CompileShaderStage(shadersource.shaders_stages[shader_compiler::ShaderType::FRAGMENT].c_str(), GL_FRAGMENT_SHADER, "FRAGMENT");
		// ----- Optional shader stages -----
		// Geometry
		GLuint geom = 0;
		if (shadersource.used_stages[shader_compiler::ShaderType::GEOMETRY])
			geom = CompileShaderStage(shadersource.shaders_stages[shader_compiler::ShaderType::GEOMETRY].c_str(), GL_GEOMETRY_SHADER, "GEOMETRY");
		
		// TODO: Add more shader stages.
		GLCall(glAttachShader(program, vert));
		GLCall(glAttachShader(program, frag));
		if (geom) { GLCall(glAttachShader(program, geom)); }
		GLCall(glDeleteShader(vert));
		GLCall(glDeleteShader(frag));
		if (geom) { GLCall(glDeleteShader(geom)); }
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));
		return program;
	}
}
