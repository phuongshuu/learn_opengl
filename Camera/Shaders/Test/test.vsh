#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform float offsetX;
uniform mat4 model;
uniform mat4 view;
uniform mat4 prj;

out vec3 fPos;
out vec2 fTexCoord;
void main() {
	gl_Position = prj * view * model * vec4(aPos, 1.0f);
	fTexCoord = aTexCoord;
}