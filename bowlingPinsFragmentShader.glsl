#version 330 core

out vec4 FragColor;  // This is the output of the fragment shader

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;  // Must be bound from C++ with a texture
uniform vec3 lightDir;               // Light direction
uniform vec3 viewPos;                // Camera/viewer position

void main()
{
    // Normalize the normal and light direction
    vec3 norm = normalize(Normal);
    vec3 light = normalize(-lightDir);

    // Diffuse lighting calculation (Lambertian reflectance)
    float diff = max(dot(norm, light), 0.0);

    // Specular lighting calculation using Phong reflection model
    vec3 viewDir = normalize(viewPos - FragPos);  // View direction
    vec3 reflectDir = reflect(-light, norm);      // Reflection direction
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);  // Specular intensity

    // Sample the texture color
    vec3 textureColor = texture(texture_diffuse1, TexCoord).rgb;

    // Modify ambient light to brighten the pin without affecting texture
    vec3 ambient = 0.9 * textureColor;  // Small increase in ambient light to brighten the pin

    // Combine ambient, diffuse, and specular lighting for final color
    vec3 color = ambient + (0.1 + diff) * textureColor + vec3(spec) * 0.5;  // The ambient light is increased slightly

    // Output the final color (frag color with alpha set to 1.0)
    FragColor = vec4(color, 1.0);
}
