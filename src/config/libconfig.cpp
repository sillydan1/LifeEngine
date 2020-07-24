/* libconfig.cpp
 * Config parsing library for Life engine
 * Adapted from ConfigParser made by underww on github
 * https://github.com/underww/ConfigParser
 * 
 * Main logic for config parsing
 * compile with:
 * 	g++ $(CFLAGS) -fPIC -c config/libconfig.cpp
 * 	g++ $(CFLAGS) -shared -Wl,-soname,libconfig.so -o bin/libconfig.so libconfig.o
 * 
 * usage:
 *  ConfigParser config("test.conf");
 *  int ValueInMemory;
 *  config.get("ValueInConfig", ValueInMemory);
 */

#include <fstream>            // ifstream & ofstream
#include <iostream>            // cout (remove later)
#include <algorithm>        // find_if
#include <cctype>            // isalpha
#include "libconfig.hpp"    // Forward declarations from header
#include <life_core.h>

// Constructor
ConfigParser::ConfigParser(const std::string& filename, bool SyncToDisk) : filename(filename) {
    std::ifstream ifs(filename);
    if(!ifs.is_open()) {
        // std::cout << "ConfigParser::ConfigParser - Unable to open " << filename << std::endl;
        LIFE_ERR("ConfigParser - Unable to open '%s'", filename.c_str());
        return;
    }

    std::string line;
    while(std::getline(ifs, line)) {
        trim(line);
        auto pair = parse(line);
        if(!pair.first.empty() && !pair.second.empty())
            configMap[pair.first] = pair.second; // overwrite if exists
    }
    ifs.close();
    sync = SyncToDisk;
    success = true;
}

// Destructer. Will sync changes to disk, if sync == true.
ConfigParser::~ConfigParser() {
    if(sync && modified) {
        ApplyChanges();
    }
}

void ConfigParser::EnableSync() {
    sync = true;
}

void ConfigParser::DisableSync() {
    sync = false;
}

// Sync changes in memory to disk
void ConfigParser::ApplyChanges() {
    std::vector<KeyValuePair> content;

    /* READ CONTENT */
    std::ifstream ifs(filename);
    if(ifs.is_open()) {
        std::string line;
        while(std::getline(ifs, line)) {
            trim(line);
            ConfigParser::KeyValuePair pair = parse(line);

            if(!pair.first.empty() && !pair.second.empty()) {
                auto found = configMap.find(pair.first);
                if(found != configMap.end())
                    pair.second = found->second;
            } else {
                pair.first = line;
                pair.second = "";
            }
            content.emplace_back(pair);
        }
        ifs.close();
    }

    // add elements that doesn't exist in a file but exist in configMap
    for(auto it = configMap.begin(); it != configMap.end(); ++it) {
        auto found = std::find_if(content.begin(), content.end(), [&](const KeyValuePair& p) {
            return p.first == it->first;
        });

        if(found == content.end()) {
            content.emplace_back(it->first, it->second);
        }
    }

    /* WRITE CONTENT */
    std::ofstream ofs(filename);
    if(!ofs.is_open()) {
        std::cout << "ConfigParser::~ConfigParser - Unable to open " << filename << std::endl;
        return;
    }

    for(const auto& element : content) {
        ofs << element.first;

        if(!element.second.empty()) {
            ofs << " = " << element.second;
        }
        ofs << std::endl;
    }
    ofs.close();
}

// Trims whitespace from string
void ConfigParser::trim(std::string& string) const {
    std::size_t first = string.find_first_not_of(" \t\n");
    std::size_t last = string.find_last_not_of(" \t\n");

    if(first == std::string::npos) { // empty string
        string = "";
    } else {
        string = string.substr(first, last - first + 1);
    }
}

// Parse string as KeyValuePair
ConfigParser::KeyValuePair ConfigParser::parse(const std::string& line) const {
    std::string key, value;
    std::size_t equal = line.find('=');

    if(equal != std::string::npos && std::isalpha(line.front())) { // !line.empty()
        key = line.substr(0, equal);
        value = line.substr(equal + 1);

        trim(key);
        trim(value);
    }
    return std::make_pair(key, value);
}
