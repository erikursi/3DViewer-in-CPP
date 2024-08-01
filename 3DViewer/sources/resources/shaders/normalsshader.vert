#version 120

attribute vec4 vertex;
attribute vec3 start_vector;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform float scale;

void main() {
    vec3 pos = vertex.rgb;
    float type = vertex.a;
    if (type > 0.0) {
        pos = pos * scale + start_vector;
    }
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(pos, 1.0);
}
