// Standard life 3d vertex shader.
#shader vertex
#INCLUDEFILE LIFE_3D_VERT

#shader fragment
#version 130

#INCLUDEFILE LIFE_MATERIAL

in vec3 Normal;
in vec3 FragPos;
in vec2 UVCoord;

out vec4 color;

uniform Material u_material;

void main() {
	color = texture(u_material.diffuse, UVCoord);
}
