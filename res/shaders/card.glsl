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
uniform vec3 u_viewPos;

#INCLUDEFILE LIFE_DEFAULTS

void main() {
    if(life_defaults()) return;

    vec3 texcolor = vec3(texture(u_material.diffuse, UVCoord));// + specColor;
    color = vec4(texcolor.xyz,1.0);//mix(tex1Color, tex2Color, 0.6);

}
