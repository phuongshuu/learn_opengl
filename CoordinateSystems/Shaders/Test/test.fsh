#version 330 core
out vec4 FragColor;
in vec3 fPos;
in vec2 fTexCoord;

uniform sampler2D img;
uniform sampler2D face;
uniform float delta;

void main() {
	//FragColor = texture(face, fTexCoord);
	FragColor = mix(texture(img, fTexCoord), texture(face, vec2(1.0f - fTexCoord.x, fTexCoord.y)), delta);
}