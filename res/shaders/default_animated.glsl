#shader vertex
#INCLUDEFILE LIFE_3D_VERT_ANIMATED

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
	// vec3 spotlightdir = vec3(0.5, -0.5, 0.0);
	// vec3 norm = normalize(Normal);
	// vec3 reflectDir = reflect(-spotlightdir, norm);
	// vec3 viewDir = normalize(u_viewPos - FragPos);
	// float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	// vec3 specColor = spec * vec3(texture(u_material.specular, UVCoord));
	vec3 texcolor = vec3(texture(u_material.diffuse, UVCoord));// + specColor;
	color = vec4(texcolor.xyz,1.0);//mix(tex1Color, tex2Color, 0.6);
}
