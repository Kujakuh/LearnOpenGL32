#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse[4];
uniform sampler2D texture_specular[4];

void main()
{   vec4 result = vec4(0.0f);
    for(int i = 0; i < 4; i++) result += texture(texture_diffuse[i], TexCoords);
    for(int i = 0; i < 4; i++) result += texture(texture_specular[i], TexCoords);
    FragColor = result;
}