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
#ifndef LIFEENGINE_SANDBOXLAYER_H
#define LIFEENGINE_SANDBOXLAYER_H
#include <glpch.h>
#include "layers/Layer.h"
#include "rendering/shader/Shader.h"

class sandbox_layer : public layer {
    GLuint vertex_buffer_object{};
    GLuint vertex_array_object{};
    Shader buffer_shader;
public:
    sandbox_layer();
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
};

#endif //LIFEENGINE_SANDBOXLAYER_H
