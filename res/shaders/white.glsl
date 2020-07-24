#shader vertex
#INCLUDEFILE LIFE_3D_VERT

#shader fragment
#version 130

in vec3 Normal;
in vec3 FragPos;
in vec2 UVCoord;

out vec4 color;

void main() {
	color = vec4(1.0,1.0,1.0,1.0);//mix(tex1Color, tex2Color, 0.6);
}
