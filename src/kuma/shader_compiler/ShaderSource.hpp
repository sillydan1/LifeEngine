#ifndef SHADERSOURCE_HPP
#define SHADERSOURCE_HPP
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "kuma/kuma_math_types.hpp"
#include <kuma/utils.h>

namespace life::kuma { 
	namespace shader_compiler {
		// Utility Functions
		enum ShaderType : char {
			NOTSHADERCODE = -1, 
			VERTEX = 0,
			FRAGMENT = 1,
			GEOMETRY = 2,
			POSTPROCESS = 3,
			SUBPOSTPROCESS = 4,
			SIZE = 5
		}; 

		struct ShaderSource {
			std::string 		shaders_stages[ShaderType::SIZE];
			bool				used_stages[ShaderType::SIZE] = {
				false,
				false,
				false,
				false,
				false
			};
			ShaderType 			current_stage = ShaderType::VERTEX;
			std::string file_path;
			
			// ---------------
			std::string GetShaderAtStage(ShaderType stage) const {
				return shaders_stages[stage];
			}
			bool SetStage(ShaderType stage) {
				if(stage >= ShaderType::SIZE) return false;
				if(stage <= ShaderType::NOTSHADERCODE) return false;
				current_stage = stage;
				return true;
			}
			void AddLine(const std::string& line) {
				this->AddLineToStage(line, current_stage);
			}
			void AddLineToStage(const std::string& line, ShaderType stage) {
				if(stage >= ShaderType::SIZE) return; // errornous adding.
				shaders_stages[stage] += line;
				shaders_stages[stage] += "\n";
				used_stages[stage] = true;
			}
		};
	}
}

#endif //CLASSNAME_HPP
