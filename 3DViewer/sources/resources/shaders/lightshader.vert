#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 light_pos;

// for lighting

varying highp vec3 Normal;
varying highp vec3 FragPos;
varying mediump vec3 LightPos;

// for textures
varying vec2 UV;


// Function definitions for transpose and inverse
mat3 transpose(mat3 m) {
    return mat3(
        m[0][0], m[1][0], m[2][0],
        m[0][1], m[1][1], m[2][1],
        m[0][2], m[1][2], m[2][2]
    );
}

float determinant(mat3 m) {
    float result = m[0][0] * m[1][1] * m[2][2] + m[0][1]*m[1][2]*m[2][0] + m[1][0] * m[2][1] * m[0][2] - m[2][0] * m[1][1] * m[0][2] - m[1][0]*m[0][1]*m[2][2] - m[0][0] * m[2][1] * m[1][2];
    return result;
}

mat3 inverse(mat3 m) {
    float det = determinant(m);
    if (det == 0.0) {
        return mat3(0.0); // Matrix is not invertible
    }

    mat3 adjoint = mat3(
        m[1][1]*m[2][2] - m[1][2]*m[2][1],
        m[0][2]*m[2][1] - m[0][1]*m[2][2],
        m[0][1]*m[1][2] - m[0][2]*m[1][1],

        m[1][2]*m[2][0] - m[1][0]*m[2][2],
        m[0][0]*m[2][2] - m[0][2]*m[2][0],
        m[0][2]*m[1][0] - m[0][0]*m[1][2],

        m[1][0]*m[2][1] - m[1][1]*m[2][0],
        m[0][1]*m[2][0] - m[0][0]*m[2][1],
        m[0][0]*m[1][1] - m[0][1]*m[1][0]
    );

    return adjoint / det;
}

void main() {
    mat4 model_view_matrix =  view_matrix * model_matrix;
    FragPos = vec3(model_view_matrix * vec4(position, 1.0));
    mat3 normal_matrix = mat3(transpose(inverse(mat3(model_view_matrix))));
    Normal = normalize(normal_matrix * normal);
    LightPos = vec3(view_matrix * vec4(light_pos, 1.0));
    UV = uv;
    gl_Position = projection_matrix * vec4(FragPos, 1.0);
}
