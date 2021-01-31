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
#include <rendering/debugging.h>
#include "Shader.h"

bool Shader::CompileAndLinkShaders(std::vector<ShaderProgram>& programs) {
    auto compilation_success = std::all_of(programs.begin(), programs.end(), [](auto&& p){ return p.Compile(); });
    if(compilation_success)
        return LinkShaders(programs);
    return false;
}

bool ShaderProgram::Compile() {
    shaderProgram = glCreateShader(Shader::ConvertShaderStageEnumToOGL(stage));
    auto* cstrptr = src.c_str();
    glShaderSource(shaderProgram, 1, &cstrptr, nullptr);
    glCompileShader(shaderProgram);
    int compilationSuccess;
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &compilationSuccess);
    if(compilationSuccess)
        return shaderProgram;
    char infoLog[512];
    glGetShaderInfoLog(shaderProgram, 512, nullptr, infoLog);
    spdlog::error("Shader stage {0} compilation failed:\n {1}", stage, infoLog);
    return false;
}

ShaderProgram::~ShaderProgram() {
    glDeleteShader(shaderProgram);
}

GLuint Shader::ConvertShaderStageEnumToOGL(const ShaderStage& stageEnum) {
    switch (stageEnum) {
        case ShaderStage::Vertex:       return GL_VERTEX_SHADER;
        case ShaderStage::Fragment:     return GL_FRAGMENT_SHADER;
    }
    throw std::logic_error("Provided ShaderStage enum is not convertible to GL_X_SHADER int");
}

bool Shader::LinkShaders(std::vector<ShaderProgram>& programs) {
    shaderProgram = glCreateProgram();
    for(auto& p : programs)
        glAttachShader(shaderProgram, p.shaderProgram);
    glLinkProgram(shaderProgram);
    int linkingSuccess;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkingSuccess);
    if(linkingSuccess) return true;
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    spdlog::error("Shader linking failed:\n {0}", infoLog);
    return false;
}

void Shader::Use() const {
    glUseProgram(shaderProgram);
}

void Shader::Destroy() const {
    glDeleteProgram(shaderProgram);
}
