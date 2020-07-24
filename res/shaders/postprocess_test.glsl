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

void main()
{
	if(true) {
		vec2 offsets[9] = vec2[](
			vec2(-offset,  offset), // top-left
			vec2( 0.0f,    offset), // top-center
			vec2( offset,  offset), // top-right
			vec2(-offset,  0.0f),   // center-left
			vec2( 0.0f,    0.0f),   // center-center
			vec2( offset,  0.0f),   // center-right
			vec2(-offset, -offset), // bottom-left
			vec2( 0.0f,   -offset), // bottom-center
			vec2( offset, -offset)  // bottom-right    
		);
		float div = 1.0;
		float kernel[9] = float[] (
		-1 / div,-1 / div, -1 / div,
		-1 / div, 10 / div, -1 / div,
		-1 / div,-1 / div, -1 / div
		);
		
		vec3 sampleTex[9];
		for	(int i = 0; i < 9; i++) {
			sampleTex[i] = vec3(texture(u_screenTexture, UVCoord.st + offsets[i]));
		}
		vec3 col = vec3(0.0);
		for	(int i = 0; i < 9; i++) {
			col += sampleTex[i] * kernel[i];
		}
		FragColor = vec4(col.r, col.g, col.b * 0.5, 1.0);
    } else {
		vec3 col2 = texture(u_screenTexture, UVCoord).rgb;
		FragColor = vec4(col2, 1.0);  // col
	}
}  

/*
 * vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = 1.0 - vec3(texture(u_screenTexture, UVCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    FragColor = 1.0 - vec4(col, 1.0);
 * */
