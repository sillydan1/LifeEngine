/**
 * Copyright (C) 2021 Asger Gitz-Johansen

   This file is part of lifeengine.

    lifeengine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    lifeengine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with lifeengine.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <lifepch.h>
#include <cctype>
#include <locale>
#include <regex>
#include "stringextensions.h"

std::vector<std::string> split(const std::string& s, char delimiter)  {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

std::vector<std::string> split(const std::string& s, const std::string& delimiter)  {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string sc = s;
    std::string token;
    while ((pos = sc.find(delimiter)) != std::string::npos) {
        token = sc.substr(0, pos);
        tokens.push_back(token);
        sc.erase(0, pos + delimiter.length());
    }
    tokens.push_back(sc);
    return tokens;
}

std::vector<std::string> regex_split(const std::string& s, const std::string& regex) {
    std::regex re(regex);
    std::sregex_token_iterator first{s.begin(), s.end(), re, -1}, last;//the '-1' is what makes the regex split (-1 := what was not matched)
    std::vector<std::string> tokens{first, last};
    return tokens;
}

std::string GetFileNameOnly(const std::string& fullpath, char delimiter) {
    auto filenameWithExtension = fullpath.substr(fullpath.find_last_of(delimiter) + 1);
    return filenameWithExtension.substr(0, filenameWithExtension.find_last_of('.'));
}

// trim from start (in place)
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

bool contains(const std::string& s, const std::string& substring) {
    return s.find(substring) != std::string::npos;
}

std::optional<const size_t> containsString(const std::string& s, const std::string& substring) {
    auto i = s.find(substring);
    if(i == std::string::npos) return {};
    return {i};
}
