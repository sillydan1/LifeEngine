#include "Precompiler.hpp"
#include <vector>
#include <kuma/utils.h>
#include <life_core.h>
#include <iostream>
#include <istream>
#include <string>
#include <stdexcept>

namespace life::kuma::shader_compiler {
	
	std::string default_directory = "";
	
	std::vector<std::string> Precompiler::ExpandMacros(std::fstream& file) {
		
		std::string line;
		std::vector<std::string> lines{};
		
		// Go through the thing line-by-line
		while(std::getline(file, line)) {
			if(line.find("#INCLUDEFILE") != std::string::npos)  {
				std::vector<std::string> kwds = split(line, ' ');
				kwds.erase(kwds.begin()); // We don't care about the '#INCLUDEFILE' keyword itself 
				for (auto& file : kwds) {
					auto stream = std::fstream(default_directory + file + ".glsl");
					if(!stream.is_open()) {
						LIFE_ERR("Could not open file with filename '%s.glsl'", file.c_str());
						// throw std::runtime_error("Could not open file");
						continue;
					}
					std::vector<std::string> expanded = Precompiler::ExpandMacros(stream);
					for(auto& l : expanded) {
						lines.push_back(l);
					}
				}
			} else {
				lines.push_back(line);
			}
		}
		return lines;
	}
	
	void Precompiler::SetDefaultShaderDirectory(const std::string& directory) {
		default_directory = directory;
	}
}
