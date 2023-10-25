#version 460 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 textCoord;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform float pick;

void main()
{
   FragColor = mix(texture(ourTexture1, textCoord), texture(ourTexture1, textCoord), pick);
};