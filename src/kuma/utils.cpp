#include "openglmath"
#include "utils.h"
#include <life_core.h>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <cctype>
#include <locale>

#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>

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
		LIFE_OGL_ERR("Error value: %s : %s", err.c_str(), funcname);
		//std::cout << " [OpenGL error]: " << err << " : " <<  funcname << std::endl;
		return false;
	}
	return true;
}

void GLClearError() {
	while(glGetError() != GL_NO_ERROR);
}

bool Vec3NumericDiff(const glm::vec3& a, const glm::vec3& b, float max_dif) {
	return std::abs(a.x - b.x) < max_dif && std::abs(a.y - b.y) < max_dif && std::abs(a.z - b.z) < max_dif;
}
