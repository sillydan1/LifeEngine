// Standard life 3d vertex shader.
#shader vertex
#INCLUDEFILE LIFE_3D_VERT

#shader fragment
#version 130

#INCLUDEFILE LIFE_MATERIAL

// Lighting models
//#define NR_POINT_LIGHTS 10
#INCLUDEFILE LIFE_LIT

vec3 CalculateDirectionalLight(const int light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(const int light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(const int light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateLight();
float LineraizeDepth(float depth);

in vec3 Normal;
in vec3 FragPos;
in vec2 UVCoord;

out vec4 color;

uniform Material u_material;
uniform vec3 u_viewPos;

float near = 0.1;
float far  = 10.0;

float amount = 30;

void main() {
	
	vec3 result = CalculateLight();

	color = vec4(result, 1.0f);
}

float LineraizeDepth(float depth) {
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

vec3 CalculateLight() {
	// properties
	
    vec3 norm = -normalize(Normal);
    vec3 viewDir = normalize(u_viewPos - FragPos);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = CalculateDirectionalLight(0, norm, viewDir);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalculatePointLight(i, norm, FragPos, viewDir);
    // phase 3: spot light
    for	(int i = 0; i < NR_SPOT_LIGHTS; i++)
		result += CalculateSpotLight(i, norm, FragPos, viewDir);   
	
	return result;
}

// lighting model function implementations
// Directional lights
vec3 CalculateDirectionalLight(const int light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-u_dirLights[light].direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(-normal, halfwayDir), 0.0), u_material.shininess);
    
    // combine results
    vec3 ambient  = u_dirLights[light].ambient  * vec3(texture(u_material.diffuse, 			UVCoord*amount));
    vec3 diffuse  = u_dirLights[light].diffuse  * diff * vec3(texture(u_material.diffuse, 	UVCoord*amount));
    vec3 specular = u_dirLights[light].specular * spec * vec3(texture(u_material.specular, 	UVCoord*amount));
    return clamp((ambient + diffuse + specular), vec3(0.0), vec3(1.0));
}
// Point lights
vec3 CalculatePointLight(const int light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(u_pointLights[light].position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    // Blinn-phone for point lights
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(-normal, halfwayDir), 0.0), u_material.shininess); // <-- Blinn
    
    // attenuation
    float distance    = length(u_pointLights[light].position - fragPos);
    float attenuation = 1.0 / (u_pointLights[light].constant + u_pointLights[light].linear * distance + 
  			     u_pointLights[light].quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = u_pointLights[light].ambient  * vec3(texture(u_material.diffuse, 			UVCoord*amount));
    vec3 diffuse  = u_pointLights[light].diffuse  * diff * vec3(texture(u_material.diffuse, 	UVCoord*amount));
    vec3 specular = u_pointLights[light].specular * spec * vec3(texture(u_material.specular, 	UVCoord*amount));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return clamp((ambient + diffuse + specular), vec3(0.0), vec3(1.0));
}
// Spot lights
vec3 CalculateSpotLight(const int light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(u_spotLights[light].position - fragPos);
	// cutoff (smooth)
	float theta = dot(lightDir, normalize(-u_spotLights[light].direction));
	float epsilon = u_spotLights[light].cutOff - u_spotLights[light].outerCutOff;
	float intensity = clamp((theta - u_spotLights[light].outerCutOff) / epsilon, 0.0, 1.0);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(-normal, halfwayDir), 0.0), u_material.shininess);
    
	// combine results
	vec3 ambient 	= u_spotLights[light].ambient		* vec3(texture(u_material.diffuse, 			UVCoord*amount));
	vec3 diffuse 	= u_spotLights[light].diffuse		* diff * vec3(texture(u_material.diffuse, 	UVCoord*amount));
	vec3 specular	= u_spotLights[light].specular	* spec * vec3(texture(u_material.specular, 		UVCoord*amount));
	ambient  *= intensity;
	diffuse  *= intensity;
	specular *= intensity;
	return clamp((ambient + diffuse + specular), vec3(0.0), vec3(1.0));
}
