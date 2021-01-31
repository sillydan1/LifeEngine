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
#include "SandboxLayer.h"
#include <rendering/basics/Mesh.h>
#include <rendering/shader/ShaderParser.h>

SandboxLayer::SandboxLayer()
: Layer("SandboxLayer"), VertexBufferObject(0)
{}

void SandboxLayer::OnAttach() {
    Layer::OnAttach();
    // Initialize OpenGL bindings
    glewInit();
    // Initialize the shader program
    bufferShader = ShaderParser{}.ParseShaderFile("../res/shaders/shader.glsl");
    // The test mesh buffer
    Mesh triangleMesh { .vertices = { Vertex{.position = glm::vec3(-0.5f, -0.5f, 0.0f) },
                                      Vertex{.position = glm::vec3( 0.5f, -0.5f, 0.0f) },
                                      Vertex{.position = glm::vec3( 0.0f,  0.5f, 0.0f) }
    } };
    glGenBuffers(1, &VertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleMesh), &triangleMesh, GL_STATIC_DRAW);

    auto attributes = Vertex::GetVertexAttributes();
    for(auto& a : attributes) a.Bind();

    bufferShader.Use();
}

void SandboxLayer::OnUpdate() {

}

void SandboxLayer::OnDetach() {
    Layer::OnDetach();
}
