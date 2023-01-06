#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform float offsetX;
uniform mat4 transform;
out vec3 fPos;
out vec2 fTexCoord;
void main() {
	gl_Position = transform * vec4(aPos, 1.0f);
	fPos = aPos;
	fTexCoord = aTexCoord;
}