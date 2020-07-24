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

uniform vec3 u_drawcolor;

out vec4 color;

void main() {
	// #9AFFFF 154.0/255.0
	color = vec4(u_drawcolor, 0.5);
}
