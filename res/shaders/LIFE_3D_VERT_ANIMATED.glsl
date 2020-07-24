#version 130
in vec3 aPos;
in vec3 aNormal;
in vec2 aUV;
in ivec4 aBoneID;
in vec4 aBoneWeight;

const int MAX_BONES = 100;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_bones[MAX_BONES];

out vec3 Normal;
out vec3 FragPos;
out vec2 UVCoord;

void main()
{
    // TODO: Something with the bones
    mat4 BoneTrans  = u_bones[aBoneID[0]] * aBoneWeight[0];
    BoneTrans      += u_bones[aBoneID[1]] * aBoneWeight[1];
    BoneTrans      += u_bones[aBoneID[2]] * aBoneWeight[2];
    BoneTrans      += u_bones[aBoneID[3]] * aBoneWeight[3];

	gl_Position = u_proj * u_view * u_model * BoneTrans * vec4(aPos, 1.0);
    vec4 NormalL = BoneTrans * vec4(aNormal, 0.0);
	Normal = mat3(transpose(-u_model)) * NormalL.xyz;
	FragPos = vec3(u_model * vec4(aPos, 1.0));
	UVCoord = aUV;
}
