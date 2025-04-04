#version 330
layout (location = 0) in vec3 position;

out vec4 vertexColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}