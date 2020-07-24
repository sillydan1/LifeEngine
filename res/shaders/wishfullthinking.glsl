#shader vertex
#version 130

in vec3 aPos;
in vec3 aNormal;
in vec2 aUV;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 Normal;
out vec3 FragPos;
out vec2 UVCoord;

void main()
{
	gl_Position = u_proj * u_view * u_model * vec4(aPos, 1.0);
	Normal = mat3(transpose(-u_model)) * aNormal;
	FragPos = vec3(u_model * vec4(aPos, 1.0));
	UVCoord = aUV;
}

#shader fragment
#version 130
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
// Lighting models
#LIFE_LIT
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
#define NR_POINT_LIGHTS 4
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
#define NR_SPOT_LIGHTS 4
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec3 Normal;
in vec3 FragPos;
in vec2 UVCoord;

out vec4 color;

uniform Material u_material;
uniform vec3 u_viewPos;
uniform PointLight 	u_pointLights[NR_POINT_LIGHTS];
uniform SpotLight 	u_spotLight;
uniform DirectionalLight u_dirLight;

void main() {
	
	// properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_viewPos - FragPos);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = CalculateDirectionalLight(u_dirLight, norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalculatePointLight(u_pointLights[i], norm, FragPos, viewDir);
    // phase 3: spot light
    result += CalculateSpotLight(u_spotLight, norm, FragPos, viewDir);   
	
	color = vec4(result, 1.0f);
}

// lighting model function implementations
// Directional lights
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(u_material.diffuse, UVCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_material.diffuse, UVCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, UVCoord));
    return clamp((ambient + diffuse + specular), vec3(0.0), vec3(1.0));
}
// Point lights
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(u_material.diffuse, UVCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(u_material.diffuse, UVCoord));
    vec3 specular = light.specular * spec * vec3(texture(u_material.specular, UVCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return clamp((ambient + diffuse + specular), vec3(0.0), vec3(1.0));
}
// Spot lights
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	// cutoff (smooth)
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	// combine results
	vec3 ambient 	= light.ambient		* vec3(texture(u_material.diffuse, UVCoord));
	vec3 diffuse 	= light.diffuse		* diff * vec3(texture(u_material.diffuse, UVCoord));
	vec3 specular	= light.specular	* spec * vec3(texture(u_material.specular, UVCoord));
	ambient  *= intensity;
	diffuse  *= intensity;
	specular *= intensity;
	return clamp((ambient + diffuse + specular), vec3(0.0), vec3(1.0));
}
