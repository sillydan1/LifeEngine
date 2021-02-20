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
#ifndef LIFEENGINE_SHADER_H
#define LIFEENGINE_SHADER_H
#include <glpch.h>
#include <lifepch.h>

enum class ShaderStage {
    Vertex = 0,
    Fragment = 1,
    MAX = Fragment
};

struct ShaderProgram {
    std::string src;
    GLuint shaderProgram;
    ShaderStage stage;
    bool Compile();
    ~ShaderProgram();
};

struct Shader {
    GLuint shaderProgram = 0;
    bool CompileAndLinkShaders(std::vector<ShaderProgram>& programs);
    static GLuint ConvertShaderStageEnumToOGL(const ShaderStage& stageEnum);
    bool LinkShaders(std::vector<ShaderProgram>& programs);
    void Use() const;
    void Destroy() const;
};

#endif //LIFEENGINE_SHADER_H
