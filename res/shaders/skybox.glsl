#shader vertex
#version 130
in vec3 aPos;

uniform mat4 u_pureview;
uniform mat4 u_proj;

out vec3 TexCoords;

void main()
{
	TexCoords = aPos;
    vec4 pos = u_proj * u_pureview * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

#shader fragment
#version 130

in vec3 TexCoords;

out vec4 color;

uniform samplerCube u_cubemap;

// Has to be included right before main.
#INCLUDEFILE LIFE_DEFAULTS

void main() {
    if(u_selectorpass) { /// The selectorcolor for skyboxes is not defined.
        color = vec4(0,0,0,1.0);
        return;
    }
    if(life_defaults()) return;
	color = texture(u_cubemap, TexCoords);
}
