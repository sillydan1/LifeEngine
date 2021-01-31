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
#include "rendering/debugging.h"

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
    glGenVertexArrays(1, &VertexArrayObject);
    glGenBuffers(1, &VertexBufferObject);
    glBindVertexArray(VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * triangleMesh.vertices.size(), triangleMesh.vertices.data(), GL_STATIC_DRAW);

    auto attributes = Vertex::GetVertexAttributes();
    for(auto& a : attributes) a.Bind();

}

void SandboxLayer::OnUpdate() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    bufferShader.Use();
    glBindVertexArray(VertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void SandboxLayer::OnDetach() {
    Layer::OnDetach();
    bufferShader.Destroy();
}
