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
#include "debugging.h"

bool GLCheckError(const char* funcname) {
    while(GLenum error = glGetError()) {
        std::string err = "";
        switch(error) {
            case GL_INVALID_OPERATION:      err="INVALID_OPERATION";      	break;
            case GL_INVALID_ENUM:           err="INVALID_ENUM";           	break;
            case GL_INVALID_VALUE:          err="INVALID_VALUE";          	break;
            case GL_OUT_OF_MEMORY:          err="OUT_OF_MEMORY";          	break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:  err="INVALID_FRAMEBUFFER_OPERATION";  break;
            default: 						err = std::to_string(error); 	break;
        }
        spdlog::error("[OpenGL error]: {0} : {1}", err, funcname);
        return false;
    }
    return true;
}

void GLClearError() {
    while(glGetError() != GL_NO_ERROR);
}
