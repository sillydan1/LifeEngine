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
#include "Shader.h"

class ShaderParser {
    Shader::ShaderStage currentStage;
    std::string line;
    std::stringstream stringstreams[static_cast<int>(Shader::ShaderStage::MAX) + 1];
public:
    Shader ParseShaderFile(const std::string& sourceFilePath);
private:
    struct PrecompilerCommandMatch {
        bool exists = false;
        Shader::ShaderStage nextStage = Shader::ShaderStage::MAX;
    };
    static PrecompilerCommandMatch ParsePrecompilerCommand(const std::string& codeline);
    void Reset();
};

#endif //LIFEENGINE_SHADERPARSER_H
