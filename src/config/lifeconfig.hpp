#ifndef LIFECONFIG_HPP
#define LIFECONFIG_HPP
#include "libconfig.hpp"
#include <sys/stat.h>
#include <life_core.h>
#include <string>
#include <vector>
#include <util/FileSystem.hpp>

namespace life {
#define LIFE_CONF_GET(x) life::LifeConfig::GetParser(x)->get
#define LIFE_CONF_SET(x) life::LifeConfig::GetParser(x)->set
#define LIFE_CONF_SET_VAL(...) LIFE_CONF_SET(getrespath()+"/conf/base.conf")(__VA_ARGS__)
#define LIFE_CONF_GET_VAL(...) LIFE_CONF_GET(getrespath()+"/conf/base.conf")(__VA_ARGS__)
#define LIFE_CONF_SAVE() life::LifeConfig::GetParser(getrespath()+"/conf/base.conf")->ApplyChanges()
	class LifeConfig {
	public:
		static LifeConfig& GetInstance() {
			static LifeConfig instance;
			return instance;
		}
		LifeConfig(LifeConfig const&) 		= delete;							// You can't copy or assign ConfigParsers
		void operator=(LifeConfig const&)	= delete;							// You can't copy or assign ConfigParsers
		
		static ConfigParser* GetParser(const std::string& file) {
			auto found = GetInstance().parserMap.find(file);						// Try to find parser instance
			if (found != GetInstance().parserMap.end()) {						// Check if result is valid
				return found->second;											// Return valid result
			}
			return GetInstance().insertNew(file);
		}
		
		bool InitAll(const std::vector<std::string>& configList) {
			uint sz = configList.size();
			bool retval = true;
			for (uint i=0;i<sz;i++) {
				if (!loadParser(configList[i])) {								// Load the config, and check for success
					retval = false;												// It failed, set return value accordingly
					LIFE_ERR("Unable to locate config file \"%s\"", configList[i].c_str());
				}
			}
			return retval;
		}
		
		void DeAllocAll() {														// Delete all entries in map
			for(auto it = parserMap.begin(); it != parserMap.end();) {			// Iterate over map
				delete it->second;												// Free each element
				parserMap.erase(it);											// Erase the entry in map
				it++;															// Increment
			}
		}
		
	private:
		// Constructor & Destructor
		LifeConfig() {}
		
		std::unordered_map<std::string, ConfigParser*> parserMap;
		std::vector<std::string> configLocations = { "./conf/", "./" };
		
		std::string absFromRelative (const std::string& file) {					// Finds absolute path from relative
			uint sz = configLocations.size();
			struct stat buffer;
			for (uint i=0;i<sz;i++) {
				std::string filename = configLocations[i]+file;
				if (stat(filename.c_str(), &buffer) == 0) {
					return filename;
				}
			}
			return file;
		}
		
		ConfigParser* insertNew(const std::string& key) {
			ConfigParser* a = new ConfigParser(absFromRelative(key), false);	// Create new parser instance
			if (a->success) {													// Check that it loaded correctly
				GetInstance().parserMap.insert(std::make_pair(key, a));			// Insert parser into map
				return a;														// Return parser we just created
			}
			LIFE_ERR("Error in parser for \"%s\"", key.c_str());				// Something went wrong
			return nullptr;	
		}
		
		bool loadParser(const std::string& key) {
			auto found = parserMap.find(key);
			if (found != parserMap.end()) {
				return found->second->success;
			}
			return insertNew(key) != nullptr;
		}
	};
}
#endif
