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
#include "layers/Layer.h"

class SandboxLayer : public Layer {
public:
    SandboxLayer();
    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
};

#endif //LIFEENGINE_SANDBOXLAYER_H
