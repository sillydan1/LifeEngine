#vert
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 inColor;

void main() {
    inColor = aColor;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

#frag
#version 330 core
out vec4 FragColor;
in vec3 inColor;

void main() {
	FragColor = vec4(inColor, 1.0); //vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
