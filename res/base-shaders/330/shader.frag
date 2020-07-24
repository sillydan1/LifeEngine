#version 330

varying vec2 texCoord0;
varying vec3 color0;
uniform sampler2D diffuse;

void main(){
	gl_FragColor = vec4(color0, 1.0);//texture2D(diffuse, texCoord0);//vec2(0.2, 0.2));//vec4(51.0/255.0, 94.0/255.0, 32.0/255.0, 1.0);
}
