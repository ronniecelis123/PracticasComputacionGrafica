#version 330 core

#define NUMBER_OF_POINT_LIGHTS 2
#define NUMBER_OF_DIR_LIGHTS    2
#define NUMBER_OF_SPOT_LIGHTS   3

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight   dirLights[NUMBER_OF_DIR_LIGHTS];
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight  spotLights[NUMBER_OF_SPOT_LIGHTS];
uniform Material   material;
uniform int        transparency;

// Prototipos
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );

void main( )
{
    vec3 norm = normalize( Normal );
    vec3 viewDir = normalize( viewPos - FragPos );

    vec3 result = vec3(0.0);

    // Direccionales
    for (int i = 0; i < NUMBER_OF_DIR_LIGHTS; ++i) {
        result += CalcDirLight(dirLights[i], norm, viewDir);
    }

    // Puntuales
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; ++i) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    // Spotlights
    for (int i = 0; i < NUMBER_OF_SPOT_LIGHTS; ++i) {
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
    }

    vec4 tex = texture(material.diffuse, TexCoords);
    color = vec4(result * tex.rgb, tex.a);
    if (color.a < 0.1 && transparency == 1)
        discard;
}

// ====== Funciones de iluminaci�n ======

vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir )
{
    vec3 lightDir = normalize( -light.direction );

    // Diffuse
    float diff = max( dot( normal, lightDir ), 0.0 );

    // Specular
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );

    // Combine
    vec3 ambient  = light.ambient  * vec3( texture( material.diffuse, TexCoords ) );
    vec3 diffuse  = light.diffuse  * diff * vec3( texture( material.diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );

    return ( ambient + diffuse + specular );
}

vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );

    // Diffuse
    float diff = max( dot( normal, lightDir ), 0.0 );

    // Specular
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );

    // Attenuation
    float distance    = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

    // Combine
    vec3 ambient  = light.ambient  * vec3( texture( material.diffuse, TexCoords ) );
    vec3 diffuse  = light.diffuse  * diff * vec3( texture( material.diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ( ambient + diffuse + specular );
}

vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );

    // Diffuse
    float diff = max( dot( normal, lightDir ), 0.0 );

    // Specular
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );

    // Attenuation
    float distance    = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

    // Spotlight intensity (soft edges)
    float theta   = dot( lightDir, normalize( -light.direction ) );
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0 );

    // Combine
    vec3 ambient  = light.ambient  * vec3( texture( material.diffuse, TexCoords ) );
    vec3 diffuse  = light.diffuse  * diff * vec3( texture( material.diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ( ambient + diffuse + specular );
}
