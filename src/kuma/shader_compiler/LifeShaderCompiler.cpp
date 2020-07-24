#include "LifeShaderCompiler.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <life_core.h>

namespace life::kuma::shader_compiler {
	
	ShaderSource LifeShaderCompiler::Parse(const std::string& file_name) {
		ShaderSource full_source{};
		full_source.file_path = file_name;
		
		const size_t last_slash_idx = file_name.rfind('/');
		if (std::string::npos != last_slash_idx) {
			Precompiler::SetDefaultShaderDirectory(file_name.substr(0, last_slash_idx) + "/");
		}
		
		std::fstream infile(file_name);
		if(!infile.is_open()) {
			LIFE_ERR("Could not open file with filename '%s'.", file_name.c_str());
			return full_source;
		}
		auto full_file = Precompiler::ExpandMacros(infile);
		// Now split into shader stages
		for(auto& line : full_file) {
			if(line.find("#shader") != std::string::npos) {
					   if(line.find("vertex") != std::string::npos) {
					full_source.SetStage(ShaderType::VERTEX); 	continue;
				} else if(line.find("fragment") != std::string::npos) {
					full_source.SetStage(ShaderType::FRAGMENT); continue;
				} // Extend here with new shader stage types.
				else {
					LIFE_ERR("Shader stage '%s' not recognized. Did you remember to do it in lower-case?", line.c_str());
				}
			}
			full_source.AddLine(line);
		}
		
		return full_source;
	}
}
