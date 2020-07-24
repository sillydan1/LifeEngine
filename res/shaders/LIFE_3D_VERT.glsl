#version 130
in vec3 aPos;
in vec3 aNormal;
in vec2 aUV;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 Normal;
out vec3 FragPos;
out vec2 UVCoord;

void main()
{
	gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);
	Normal = mat3(transpose(-u_model)) * aNormal;
	FragPos = vec3(u_model * vec4(aPos, 1.0));
	UVCoord = aUV;
}
