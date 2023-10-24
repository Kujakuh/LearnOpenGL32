#version 460 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 textCoord;

uniform sampler2D ourTexture;
uniform vec4 modColor;

void main()
{
   FragColor = texture(ourTexture, textCoord);
};