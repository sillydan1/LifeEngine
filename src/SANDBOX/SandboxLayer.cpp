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

sandbox_layer::sandbox_layer() : layer("SandboxLayer"), vertex_buffer_object(0) {

}

void sandbox_layer::on_attach() {
    layer::on_attach();
    // Initialize OpenGL bindings
    glewInit();
    // Initialize the shader program
    buffer_shader = ShaderParser::ParseShaderFile("res/shaders/shader.glsl");
    // The test mesh buffer
    Mesh triangleMesh { .vertices = { Vertex{.position = glm::vec3(-0.5f, -0.5f, 0.0f),
                                             .color    = glm::vec3( 0.0f,  0.0f, 1.0f)},
                                      Vertex{.position = glm::vec3( 0.5f, -0.5f, 0.0f),
                                             .color    = glm::vec3( 0.0f,  1.0f, 0.0f)},
                                      Vertex{.position = glm::vec3( 0.0f,  0.5f, 0.0f),
                                             .color    = glm::vec3( 1.0f,  0.0f, 0.0f)}
    } };
    // Create and bind a vertex array object, so binding is easy later
    glGenVertexArrays(1, &ertex_array_object);
    glGenBuffers(1, &vertex_buffer_object);
    glBindVertexArray(ertex_array_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * triangleMesh.vertices.size(), triangleMesh.vertices.data(), GL_STATIC_DRAW);
    // Bind the vertex attributes to the vertex array object
    auto attributes = Vertex::GetVertexAttributes();
    for(auto& a : attributes)
        a.Bind();
    // Unbind the buffer again
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void sandbox_layer::on_update() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    buffer_shader.Use();
    glBindVertexArray(ertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void sandbox_layer::on_detach() {
    layer::on_detach();
    buffer_shader.Destroy();
}
