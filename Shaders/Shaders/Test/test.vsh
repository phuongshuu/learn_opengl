#version 330 core
layout(location = 0) in vec3 aPos;
uniform float offsetX;
out vec3 fPos;
void main() {
	gl_Position = vec4(aPos, 1.0f);
	gl_Position.x += offsetX;
	fPos = gl_Position.xyz;
}