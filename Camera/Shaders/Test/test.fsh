#version 330 core
out vec4 FragColor;
in vec2 fTexCoord;

uniform sampler2D img;
uniform sampler2D face;

void main() {
	FragColor = mix(texture(img, fTexCoord), texture(face, vec2(1.0f - fTexCoord.x, fTexCoord.y)), 0.2);
}