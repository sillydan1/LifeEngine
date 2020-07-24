#shader vertex
#version 130
in vec3 aPos;
in vec3 aNormal;
in vec2 aUV;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
	gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);
}

#shader fragment
#version 130

#INCLUDEFILE LIFE_MATERIAL

out vec4 color;

void main() {
	// #9AFFFF 154.0/255.0
	color = vec4(154.0/255.0, 1.0, 1.0, 0.5);
}
