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
#include "Vertex.h"

GLuint TO_GL_BOOL(bool b) { return b ? GL_TRUE : GL_FALSE; }

std::vector<VertexAttribute> Vertex::GetVertexAttributes() {
    std::vector<VertexAttribute> attrs{};
    attrs.push_back(VertexAttribute{
            .layoutLocation = 0,
            .size = 3,
            .dataType = GL_FLOAT,
            .normalized = false,
            .stride = sizeof(glm::vec3),
            .offset = 0
    });
    return attrs;
}
// Casting offset to void* is necessary for OpenGL, but it produces a warning.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
void VertexAttribute::Bind() {
    glVertexAttribPointer(layoutLocation, size, dataType, TO_GL_BOOL(normalized), stride, (void*)offset);
    glEnableVertexAttribArray(layoutLocation);
}
#pragma clang diagnostic pop
