
#version 460 core

out vec4 FragColor;

in vec2 textCoord;
in vec3 fragPos;
in vec3 normal;

uniform vec3 lightSourcePos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform float pick;

void main()
{
   float lightDistanceStrength = 6.0;
   float specularStrength = 0.1;
   int shininess = 32;
   float ambientLightVal = 0.1;

   vec3 norm = normalize(normal);
   vec3 lightDir = normalize(lightSourcePos - fragPos);
   vec3 viewDir = normalize(viewPos - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);

   float lightIntensity = min(lightDistanceStrength/2, lightDistanceStrength/length(lightSourcePos - fragPos));

   float specular = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

   vec3 ambientLightMod = vec3(ambientLightVal);
   vec3 diffuseLightMod = max(dot(norm, lightDir), 0.0) * lightColor;
   vec3 specularLightMod = specularStrength * specular * lightColor;

   FragColor = mix(texture(ourTexture0, textCoord), texture(ourTexture1, textCoord), pick)
   * vec4((ambientLightMod + diffuseLightMod + specularLightMod) * lightIntensity, 1.0);
};