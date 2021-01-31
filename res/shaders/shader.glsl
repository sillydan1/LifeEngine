#vert
#version 330

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 color;
attribute mat4 MVP;

varying vec2 texCoord0;
varying vec3 color0;

void main() {
	gl_Position = MVP * vec4(position, 1.0);
	texCoord0 = texCoord;
	color0 = color;
}

#frag
#version 330

varying vec2 texCoord0;
varying vec3 color0;
uniform sampler2D diffuse;

void main(){
	gl_FragColor = vec4(color0, 1.0);
}
