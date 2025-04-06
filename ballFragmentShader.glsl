#version 330
out vec4 color;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main() {
	color = texture(texture_diffuse1, TexCoords);
}