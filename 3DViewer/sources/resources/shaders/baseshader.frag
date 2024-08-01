#version 120

varying vec2 UV;
uniform vec3 color;
uniform bool texture_mode;
uniform bool material_mode;
uniform sampler2D textureSampler;

struct Material {
    vec3 diffuse_color;
    bool has_diffuse_texture;
    sampler2D diffuse_sampler;
    bool has_ambient_texture;
    sampler2D ambient_sampler;
};
uniform Material material;

void main() {
    if (material_mode) {
        vec4 final_color = vec4(material.diffuse_color, 1.0);
        if (material.has_diffuse_texture) {
            final_color = texture2D(material.diffuse_sampler, UV);
        }
        gl_FragColor = final_color;
    } else if (texture_mode) {
        gl_FragColor = texture2D(textureSampler, UV);
    } else {
        gl_FragColor = vec4(color, 1.0);
    }
}
