/* libconfig.hpp
 * Config parsing library for Life engine
 * Adapted from ConfigParser made by underww on github
 * https://github.com/underww/ConfigParser
 * 
 * Header file including forward declarations for libconfig.cpp
 */

#ifndef LIBCONFIG_HPP
#define LIBCONFIG_HPP
#include <sstream>            // Stringstream
#include <ios>                // boolalpha
#include <unordered_map>    // configMap
#include <vector>            // std::vector
#include <util/FileSystem.hpp>
#include <cstring>

class ConfigParser {
public:
    ConfigParser(const std::string& filename, bool SyncToDisk = false);
    ~ConfigParser();
    void ApplyChanges();
    void EnableSync();
    void DisableSync();

    template<typename T, typename... Args>
    void get(const std::string& key, T& value, Args& ... args) const;
    template<typename T>
    void get(const std::string& key, std::vector<T>& vector) const;

    template<typename T, typename... Args>
    void set(const std::string& key, const T& value, const Args& ... args);
    template<typename T>
    void set(const std::string& key, const std::vector<T>& vector);

    bool success = false;
private:
    typedef std::pair<std::string, std::string> KeyValuePair;
    template<typename T>
    void toValue(std::string& str, T& value) const;
    template<typename T>
    void toValue(std::istringstream& iss, T& value) const;

    void trim(std::string& string) const; // trim whitespace from string
    KeyValuePair parse(const std::string& line) const;
    bool modified = false;
    bool sync = false; // Should the instance sync with the file on disk when destroyed?
    std::string filename;
    std::unordered_map<std::string, std::string> configMap;
};

template<typename T>
void ConfigParser::toValue(std::string& str, T& value) const {
    auto f = str.find("@RESDIR@");
    if(f != std::string::npos) {
    	str = life::getrespath() + str.substr(f+8);
    }
    std::istringstream iss(str);
    iss >> std::boolalpha >> value;
}

template<typename T>
void ConfigParser::toValue(std::istringstream& iss, T& value) const {
    std::string str;
    if(std::getline(iss, str, ',')) {
        toValue(str, value);
    }
}

template<typename T, typename... Args>
void ConfigParser::get(const std::string& key, T& value, Args& ... args) const {
    auto found = configMap.find(key);
    if(found != configMap.end()) {
        std::istringstream iss(found->second);
        toValue(iss, value);
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic push
        int dummy[] = {0, (toValue(iss, args), 0) ...}; // parameter pack expansion
#pragma GCC diagnostic pop
    }
}

/*template <>
void ConfigParser::get<std::string>(const std::string& key, std::string& value) const {
    auto found = configMap.find(key);
    if (found != configMap.end()) {
        value = found->second;
	}
}*/

template<typename T>
void ConfigParser::get(const std::string& key, std::vector<T>& vector) const {
    auto found = configMap.find(key);
    if(found != configMap.end()) {
        std::istringstream iss(found->second);
        std::string str;
        while(std::getline(iss, str, ',')) {
            vector.emplace_back();
            std::istringstream(str) >> std::boolalpha >> vector.back();
        }
    }
}

template<typename T, typename... Args>
void ConfigParser::set(const std::string& key, const T& value, const Args& ... args) {
    std::ostringstream oss;
    oss << value;
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic push
    int dummy[] = {0, (oss << ", " << args, 0) ...}; // parameter pack expansion
#pragma GCC diagnostic pop

    if(configMap[key] != oss.str()) {
        configMap[key] = oss.str();
        modified = true;
    }
}

template<typename T>
void ConfigParser::set(const std::string& key, const std::vector<T>& vector) {
    std::ostringstream oss;
    for(auto it = vector.begin(); it != vector.end(); ++it) {
        if(it != vector.begin())
            oss << ", ";
        oss << *it;
    }

    if(configMap[key] != oss.str()) {
        configMap[key] = oss.str();
        modified = true;
    }
}

#endif
