#version 120

attribute vec3 position;
attribute vec2 uv;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
varying vec2 UV;

void main() {
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);
    UV = uv;

}
