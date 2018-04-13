#version 420

in vec3 vertex_position;
out vec4 pos;

void main() {
  gl_Position = vec4(vertex_position, 1.0);
  pos = vec4(vertex_position, 1.0);
}