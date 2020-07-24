#shader vertex
#version 130
in vec3 aPos;
in vec3 aNormal;
in vec2 aUV;

out vec2 UVCoord;

void main()
{
	gl_Position = vec4(aPos.xy, 0.0, 1.0);
	UVCoord = aUV;
}

#shader fragment
#version 130
out vec4 FragColor;
in vec2 UVCoord;
uniform sampler2D u_screenTexture;
const float offset = 1.0 / 500.0;

void main() {
    // This is a crazy simple shader. Just output the texture.
    vec3 col2 = texture(u_screenTexture, UVCoord).rgb;
    FragColor = vec4(col2, 1.0);  // col
}
