#version 120

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 color;

varying vec2 texCoord0;
varying vec3 color0;

void main() {
	gl_Position = proj * view * model * vec4(position, 1.0);
	texCoord0 = texCoord;
	color0 = color;
}
