#ifndef SHADER_HPP
#define SHADER_HPP
#include "utils.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "shader_compiler/ShaderSource.hpp"
#include "kuma_math_types.hpp"
#include <glm/glm.hpp>

namespace life::kuma {
	
	class Shader {
	private:
		GLuint m_programID;
		std::string m_filepath; // for debugging purposes.
	public:
		Shader();
		Shader(const std::string& filepath);
		~Shader();

		void SetUniform(GLint location, const KumaTypes& param) const;
		
		void Bind() const;
		void Unbind() const;
		void Compile(const shader_compiler::ShaderSource& sources);
		const char* GetFilePath() const { return m_filepath.c_str(); }
		
		GLint GetUniformLocation(const std::string& name) const;
	};
	// Shader Loading functions
	GLuint CompileShaders(const shader_compiler::ShaderSource& shadersource);
}

#endif //SHADER_HPP
