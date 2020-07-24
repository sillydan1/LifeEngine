#include "cli.h"
#include <life_core.h>

namespace life::cli {
	
	std::shared_ptr<std::stringstream> default_out_stream = std::make_shared<std::stringstream>();
	std::shared_ptr<std::ostream> current_stream = default_out_stream;
	std::map<std::string, std::function<CliReturnValue(std::vector<std::string>)>> command_map{};
	
	std::shared_ptr<std::stringstream> CLI::get_default_out_stream() {
		return default_out_stream;
	}
	
	std::map<std::string, std::function<CliReturnValue(std::vector<std::string>)>> CLI::get_command_map() {
		return command_map;
	}
	
	std::shared_ptr<std::ostream> CLI::get_current_stream() {
		return current_stream;
	}
	
	void CLI::initialize() {
	
	}
	
	void ExecuteCLICommand(const std::string& command) {
		ExecuteCLICommand(command, CLI::get_default_out_stream());
	}
	void ExecuteCLICommand(const std::string& command, const std::shared_ptr<std::ostream>& stream) {
		ExecuteCLICommand(command, stream, nullptr);
	}
	void ExecuteCLICommand(const std::string& command, const std::shared_ptr<std::ostream>& stream, std::shared_ptr<CliReturnValue> out) {
		ExecuteCLICommand(command, stream, out.get());
	}
	void ExecuteCLICommand(const std::string& command, const std::shared_ptr<std::ostream>& stream, CliReturnValue* out) {
		// Split the command up into arguments
		current_stream = stream;
		CliReturnValue v = CliReturnValue::Error;
		auto command_args = split(command, ' ');
		if(command_args.empty()) return;
		if(CLI::get_command_map().find(command_args[0]) != CLI::get_command_map().end()) {
			v = CLI::get_command_map()[command_args[0]](command_args);
		}
		if(out) *out = v;
	}
	
	LIFE_CLI_FUNCTION(Help, "Displays a variety of available commands") {
		// args (Command-name, arguments)
		// TODO: Go through all LIFE_CLI_FUNCTIONs and display their names and descriptions
		
		if(args.size() > 1) {
			return CliReturnValue::InvalidArgument;
		}
		return CliReturnValue::NotImplemented;
	}
	
	
}

