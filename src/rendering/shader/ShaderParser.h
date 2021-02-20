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
#ifndef LIFEENGINE_SHADERPARSER_H
#define LIFEENGINE_SHADERPARSER_H
#include <lifepch.h>
#include <utility>
#include "Shader.h"

class ShaderParser {
public:
    static Shader ParseShaderFile(const std::string& sourceFilePath);

private:
    enum class PrecompilerCommandType : unsigned int {
        None = 0,
        NextStage = 1,
        IncludeFile = 2
    };
    struct PrecompilerCommandMatch {
        bool exists = false;
        PrecompilerCommandType type = PrecompilerCommandType::None;
        ShaderStage nextStage = ShaderStage::MAX;
        std::string includeFile;

        PrecompilerCommandMatch(const PrecompilerCommandType& type, const ShaderStage& shaderStage)
                : exists{true}, type{type}, nextStage{shaderStage} {}
        PrecompilerCommandMatch(const PrecompilerCommandType& type, std::string  includeFile)
                : exists{true}, type{type}, includeFile{std::move(includeFile)} {}
        PrecompilerCommandMatch() : exists{false} {}
    };

    ShaderStage currentStage = ShaderStage::Vertex;
    std::stringstream shaderStageStringStreams[static_cast<int>(ShaderStage::MAX) + 1];

    static Shader ConstructShader(std::stringstream* stringStreamArray);
    static PrecompilerCommandMatch ParsePrecompilerCommand(const std::string& codeLine);
    void InterpretPrecompilerCommand(const PrecompilerCommandMatch& commandMatch);
    void ParseFile(const std::string& sourceFilePath);
};

#endif //LIFEENGINE_SHADERPARSER_H
