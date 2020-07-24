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
	// If blending is enabled:
	
	// Simple cutout:
	vec4 texColor = texture(u_material.diffuse, UVCoord);
	// if(texColor.a < 0.1)
	// 	discard;
	color = texColor;
}
