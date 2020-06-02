#version 450

uniform layout(location = 6) mat4 MVPMatrix;

in layout(location = 0) vec3 vertex;

void main() {
	gl_Position = MVPMatrix * vec4(vertex, 1.0);
}