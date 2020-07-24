#shader vertex
#version 130
in vec3 aPos;
in vec3 aNormal;
in vec2 aUV;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;
uniform vec3 u_offset;

uniform vec4 u_color;
uniform float u_scale;

out vec2 UVCoord;
out vec3 Normal;
out vec4 ParticleColor;

vec3 billboard(vec4 pos) {
    float scale = 1.0f;
    vec2 BillboardSize = vec2(1,1);
    vec3 CameraRight_worldspace = vec3(u_view[0][0], u_view[1][0], u_view[2][0]);
    vec3 CameraUp_worldspace = vec3(u_view[0][1], u_view[1][1], u_view[2][1]);
    // vec3 vertexPosition_worldspace = (pos.xyz * scale);
    vec3 vertexPosition_worldspace = pos.xyz + (CameraRight_worldspace * aPos.x * BillboardSize.x);
    vertexPosition_worldspace +=                CameraUp_worldspace    * aPos.y * BillboardSize.y;
    return vertexPosition_worldspace * scale;
}

void main() {
    // (aPos * scale) + u_offset
    gl_Position = u_proj * u_view * vec4(billboard(u_model * vec4(u_offset, 1.0)), 1.0);
    UVCoord = aUV;
    Normal = aNormal;
    ParticleColor = u_color;
}

#shader fragment
#version 130
in vec2 UVCoord;
in vec3 Normal;
in vec4 ParticleColor;

out vec4 color;

uniform sampler2D u_sprite;

#INCLUDEFILE LIFE_DEFAULTS

void main() {
    if(life_defaults()) return;

    color = texture(u_sprite, UVCoord) * ParticleColor;
}
