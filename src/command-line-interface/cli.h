#ifndef LIFE_ENGINE_CLI_H
#define LIFE_ENGINE_CLI_H
#include <string>
#include <memory>
#include <sstream>
#include <map>
#include <functional>
#include <vector>
#include <util/ppcounter.h>

namespace life::cli {
	
	enum class CliReturnValue {
		Success = 0,
		Failure = 1,
		Error = 2,
		InvalidArgument = 3,
		Warning = 4,
		NotImplemented = 5
		// etc.
	};
	
#define LIFE_CLI_FUNCTION(name, description) life::cli::CliReturnValue name(const std::vector<std::string>& args)
	
	struct CLI {
		static std::shared_ptr<std::stringstream> get_default_out_stream();
		static std::map<std::string, std::function<CliReturnValue(std::vector<std::string>)>> get_command_map();
		static std::shared_ptr<std::ostream> get_current_stream();
		
		std::ostream& operator<<(const std::string& a) {
		    return *CLI::get_current_stream() << a;
		}
		
	private:
		void initialize(); // Call this once opun startup
	};
	
	void ExecuteCLICommand(const std::string& command); // Uses the default outputstream
	void ExecuteCLICommand(const std::string& command, const std::shared_ptr<std::ostream>& stream);
	void ExecuteCLICommand(const std::string& command, const std::shared_ptr<std::ostream>& stream, std::shared_ptr<CliReturnValue> out);
	void ExecuteCLICommand(const std::string& command, const std::shared_ptr<std::ostream>& stream, CliReturnValue* out);
}

#endif //LIFE_ENGINE_CLI_H
