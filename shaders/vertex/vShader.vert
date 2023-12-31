
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord;
layout (location = 2) in vec3 aNormal;


out vec2 textCoord;
out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrixTransform;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    textCoord = vec2(aTextCoord.x, aTextCoord.y);
    normal = normalMatrixTransform * aNormal;
    fragPos = vec3(model * vec4(aPos, 1.0));
};