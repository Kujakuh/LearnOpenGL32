
#version 460 core

struct Material {
   sampler2D diffuse;
   sampler2D specularMap;
   float shininess;
};

struct Light {
   vec3 position;
   vec3 direction;
   float innerCutOff;
   float outerCutOff;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};


out vec4 FragColor;

in vec2 textCoord;
in vec3 normal;
in vec3 fragPos;

uniform Material m[];
uniform Light light;

uniform vec3 viewPos;

void main()
{
   vec3 norm = normalize(normal);
   vec3 lightDir = normalize(light.position - fragPos);
   vec3 viewDir = normalize(viewPos - fragPos);
   vec3 reflectDir = reflect(-lightDir, norm);

   float diffuse = max(dot(norm, lightDir), 0.0);
   float specular = pow(max(dot(viewDir, reflectDir), 0.0), m[0].shininess);

   vec3 ambientLightMod = light.ambient * vec3(texture(m[0].diffuse, textCoord));
   vec3 diffuseLightMod = light.diffuse * (diffuse * vec3(texture(m[0].diffuse, textCoord)));
   vec3 specularLightMod = light.specular * (specular * vec3(texture(m[0].specularMap, textCoord)));

   float distance = length(light.position - fragPos);
   float attenuation = 1.0/ (light.constant + light.linear * distance + light.quadratic * (distance * distance));

   float theta = dot(lightDir, normalize(-light.direction));
   float epsilon = light.innerCutOff - light.outerCutOff;
   float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

   FragColor = vec4((ambientLightMod + diffuseLightMod + specularLightMod) * attenuation, 1.0);
   
   //FragColor = vec4((ambientLightMod + diffuseLightMod + specularLightMod) * attenuation * intensity, 1.0);

};