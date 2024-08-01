#version 120

// base color
uniform vec3 color;
uniform bool neon_mode;


// for lighting
varying vec3 FragPos;
varying vec3 Normal;
varying vec3 LightPos;

// for textures
varying vec2 UV;
uniform bool texture_mode;
uniform bool material_mode;
uniform sampler2D textureSampler;


uniform vec3 view_pos;

struct Light{
    vec3 light_color;
    float ambient_intensity;
    float diffuse_intensity;
    float specular_intensity;
};

struct Material {
    bool has_ambient_texture;
    bool has_diffuse_texture;
    bool has_specular_texture;
    sampler2D ambient_sampler;
    sampler2D diffuse_sampler;
    sampler2D specular_sampler;
    sampler2D bump_sampler;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float shininess;
    float bump_multiplier;
    float dissolve;
};


uniform Light light;
uniform Material material;

void main() {
       float default_ambient_k = 0.2;
       float default_diffuse_k = 0.85;
       float default_specular_k = 0.5;
       float default_shininess = 16.0;
       vec3 white_color = vec3(1.0);
       vec3 black_color = vec3(0.0);

       vec3 ambient_contribution = white_color * light.ambient_intensity;
       vec3 diffuse_contribution = white_color * light.diffuse_intensity;
       vec3 specular_contribution = white_color * light.specular_intensity;

       vec3 ambient_color = white_color;
       vec3 diffuse_color = white_color;
       vec3 specular_color = white_color;
       float shininess = default_shininess;

       float bump_value = texture2D(material.bump_sampler, UV).r;

       vec3 Normal0 = normalize(Normal);
       if (material_mode) {
           Normal0 = normalize(Normal + bump_value * material.bump_multiplier);
       }
       if (!gl_FrontFacing) {
           Normal0 = -Normal0;
       }

       if (material_mode) {

           if (material.has_diffuse_texture) {
               diffuse_color *= texture2D(material.diffuse_sampler, UV).rgb;
           }

           if (material.diffuse_color != black_color) {
               diffuse_color *= material.diffuse_color;
           }

           if (neon_mode) diffuse_color = (diffuse_color * 0.75  + Normal0 * 0.55);

           if (material.has_ambient_texture) {
               ambient_color *= texture2D(material.ambient_sampler, UV).rgb * default_ambient_k;
           } else if (material.ambient_color == black_color || material.ambient_color == white_color) {
               ambient_color *= diffuse_color;
               if (material.diffuse_color == white_color) ambient_color *= default_ambient_k;
           }

           if (material.has_specular_texture) {
               specular_color *= texture2D(material.specular_sampler, UV).rgb;
           } else if (material.specular_color == black_color || material.specular_color == white_color){
               specular_color *= diffuse_color * default_specular_k;
           }

           if (material.specular_color != black_color) {
              specular_color *= material.specular_color;
           }

           if (material.ambient_color != black_color) {
              ambient_color *= material.ambient_color;
           }
           shininess = material.shininess;
       } else {

           vec3 base_color = (texture_mode) ?  texture2D(textureSampler, UV).rgb : color;
           if (neon_mode) {
                   base_color = (base_color * 0.75 + Normal0 * 0.55);
           }

           ambient_color = base_color * default_ambient_k;
           diffuse_color = base_color * default_diffuse_k;
           specular_color = base_color * default_specular_k;
       }

       diffuse_contribution *=  diffuse_color;
       ambient_contribution *= ambient_color;
       specular_contribution *=  specular_color;


       vec3 light_direction = normalize(LightPos - FragPos);
       vec3 view_direction = normalize(view_pos - FragPos);

       // Diffuse contribution
      float diffuse_factor = max(dot(Normal0, light_direction), 0.0);
      diffuse_contribution *= diffuse_factor;

       // Specular contribution
      vec3 reflect_direction = reflect(-light_direction, Normal0);
      float specular_factor = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
      specular_contribution *= specular_factor;

      // Total light contribution
      vec3 lighting = light.light_color * (ambient_contribution + diffuse_contribution + specular_contribution);

      gl_FragColor = vec4(lighting, 1.0);

}
