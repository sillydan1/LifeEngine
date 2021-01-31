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
#include <lifepch.h>
#include "shader/ShaderParser.h"

int main(int argc, char** argv) {
    if(argc < 2) return -1;
    auto shader = ShaderParser{}.ParseShaderFile(argv[1]);
    std::cout << "====== VERTEX: ======\n" << shader.vertexShader.src;
    std::cout << "======= FRAG: =======\n" << shader.fragShader.src;
    return 0;
}
