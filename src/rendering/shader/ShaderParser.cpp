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
#include "ShaderParser.h"
#include "extensions/stringextensions.h"

Shader ShaderParser::ParseShaderFile(const std::string& sourceFilePath) {
    std::ifstream infile(sourceFilePath);
    currentStage = ShaderStage::Vertex;
    while(std::getline(infile, line)) {
        auto precompilermatch = ParsePrecompilerCommand(line);
        if(precompilermatch.exists) {
            currentStage = precompilermatch.nextStage;
            continue;
        }
        stringstreams[static_cast<int>(currentStage)] << line << "\n";
    }
    auto shader = ConstructShader();
    Reset();
    return shader;
}

ShaderParser::PrecompilerCommandMatch ShaderParser::ParsePrecompilerCommand(const std::string& codeline) {
    if(contains(codeline, "#vert")) return {.exists = true, .nextStage = ShaderStage::Vertex};
    if(contains(codeline, "#frag")) return {.exists = true, .nextStage = ShaderStage::Fragment};
    return {};
}

void ShaderParser::Reset() {
    line.clear();
    currentStage = ShaderStage::Vertex;
    for(auto& ss : stringstreams) ss.clear();
}

Shader ShaderParser::ConstructShader() {
    auto shader = Shader {};
    std::vector<ShaderProgram> programs;
    for(int i = 0; i < static_cast<int>(ShaderStage::MAX) + 1; i++) {
        if(stringstreams[i].str().empty()) continue;
        programs.push_back(ShaderProgram{.src = stringstreams[i].str(),   .stage = static_cast<ShaderStage>(i)});
    }
    shader.CompileAndLinkShaders(programs);
    return shader;
}
