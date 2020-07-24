#shader vertex
#version 130

in vec3 aPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
	gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);
}

#shader fragment
#version 130

out vec4 color;

uniform vec3 u_outcolor;

void main() {
	color = vec4(u_outcolor, 1.0);
}