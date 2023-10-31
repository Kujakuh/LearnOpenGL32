#version 460 core

out vec4 FragColor;

in vec2 textCoord;
in vec3 normal;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

void main()
{
   //if(pick == 1) FragColor = texture(ourTexture1, textCoord);
   //else FragColor = texture(ourTexture0, textCoord);
   FragColor = vec4(1.0);
};  