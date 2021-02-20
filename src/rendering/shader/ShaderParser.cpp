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
    auto shaderParserInstance = ShaderParser{};
    shaderParserInstance.ParseFile(sourceFilePath);
    auto shader = ConstructShader(shaderParserInstance.shaderStageStringStreams);
    return shader;
}

void ShaderParser::ParseFile(const std::string &sourceFilePath) {
    std::ifstream infile(sourceFilePath);
    if(!infile.is_open())
        return spdlog::error("Could not open file {0}", sourceFilePath);

    std::string line;
    while(std::getline(infile, line)) {
        auto precompilerCommand = ParsePrecompilerCommand(line);
        if(precompilerCommand.exists)
            InterpretPrecompilerCommand(precompilerCommand);
        else
            shaderStageStringStreams[static_cast<int>(currentStage)] << line << "\n";
    }
}

ShaderParser::PrecompilerCommandMatch ShaderParser::ParsePrecompilerCommand(const std::string& codeLine) {
    if(contains(codeLine, "#vert"))    return PrecompilerCommandMatch{PrecompilerCommandType::NextStage, ShaderStage::Vertex};
    if(contains(codeLine, "#frag"))    return PrecompilerCommandMatch{PrecompilerCommandType::NextStage, ShaderStage::Fragment};
    if(contains(codeLine, "#include")) return PrecompilerCommandMatch{PrecompilerCommandType::IncludeFile, split(codeLine, ' ')[1]};
    return PrecompilerCommandMatch{};
}

void ShaderParser::InterpretPrecompilerCommand(const ShaderParser::PrecompilerCommandMatch& commandMatch) {
    switch (commandMatch.type) {
        case PrecompilerCommandType::NextStage:
            currentStage = commandMatch.nextStage;
            break;
        case PrecompilerCommandType::IncludeFile:
            ParseFile(commandMatch.includeFile);
            break;
        default:
            spdlog::error("Precompiler command is not recognized!");
    }
}

Shader ShaderParser::ConstructShader(std::stringstream* stringStreamArray) {
    auto shader = Shader {};
    std::vector<ShaderProgram> programs;
    for(int i = 0; i < static_cast<int>(ShaderStage::MAX) + 1; i++) {
        if(stringStreamArray[i].str().empty())
            continue;
        programs.push_back(ShaderProgram{.src = stringStreamArray[i].str(), .stage = static_cast<ShaderStage>(i)});
    }
    bool compilation_success = shader.CompileAndLinkShaders(programs);
    if(!compilation_success)
        spdlog::error("Shader compilation failed.");
    return shader;
}
