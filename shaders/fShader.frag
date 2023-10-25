#version 460 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 textCoord;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform float pick;

void main()
{
   //if(pick == 1) FragColor = texture(ourTexture1, textCoord);
   //else FragColor = texture(ourTexture0, textCoord);
   FragColor = mix(texture(ourTexture0, textCoord), texture(ourTexture1, textCoord), pick);
};