#ifndef LIFESHADERCOMPILER_HPP
#define LIFESHADERCOMPILER_HPP
#include <string>
#include "ShaderSource.hpp"
#include "Precompiler.hpp"

namespace life::kuma::shader_compiler {

	class LifeShaderCompiler {
	public:
		// You're not supposed to instantiate compilers. You just use them.
		LifeShaderCompiler() = delete; 
		~LifeShaderCompiler() = delete;
		
		static ShaderSource Parse(const std::string& filename);
	
	};
}

#endif //LIFESHADERCOMPILER_HPP
