#ifndef BASICLEXER_HPP
#define BASICLEXER_HPP
#include <string>
#include <vector>

namespace life::kuma::shader_compiler {
	struct Precompiler {
		static std::vector<std::string> ExpandMacros(std::fstream& file);
		static void SetDefaultShaderDirectory(const std::string& directory);
	};
}

#endif //CLASSNAME_HPP
