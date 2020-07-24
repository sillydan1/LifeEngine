#shader vertex
#version 130
in vec3 aPos;
in vec3 aNormal;
in vec2 aUV; // ignored.

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 Normal;
out vec3 FragPos;

void main()
{
	Normal = mat3(transpose(-u_model)) * aNormal;
	FragPos = vec3(u_model * vec4(aPos, 1.0));
	gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);
}

#shader fragment
#version 130

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 u_viewPos;
uniform samplerCube u_skybox;

void main() {
	vec3 I = normalize(FragPos - u_viewPos);
	vec3 R = reflect(I, normalize(Normal));
	color  = vec4(texture(u_skybox, R).rgb, 1.0);
}
