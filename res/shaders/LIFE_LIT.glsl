/*
 * Default Lighting structures and definitions
 * for Life Engine. Please only edit this if you know what you're doing.
 * If changes are made to this, you should recompile the rendering engine component,
 * since the structures defined here are used internally.
 */
 
#ifndef __cplusplus
// Makes the uniforms defined in this file UQACC.
#MACRO UQACC
#endif

#ifndef NR_POINT_LIGHTS
#define NR_POINT_LIGHTS 8
#endif
#ifndef NR_SPOT_LIGHTS
#define NR_SPOT_LIGHTS 4
#endif
#ifndef NR_DIR_LIGHTS
#define NR_DIR_LIGHTS 4
#endif

struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
#ifdef __cplusplus
	DirectionalLight(vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular);
	~DirectionalLight();
#endif
};

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
#ifdef __cplusplus
	PointLight(vec3 position, vec3 ambient, vec3 diffuse, vec3 specular, float constant, float linear, float quadratic);
	~PointLight();
#endif
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float cutOff;
	float outerCutOff;
#ifdef __cplusplus
	SpotLight(vec3 position, vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular, float cutOff, float outerCutOff);
	~SpotLight();
#endif
};

#ifdef __cplusplus
// This is the order of the struct definitions
#define UQACC_MAJ_DIR 0
#define UQACC_MAJ_POINT 1
#define UQACC_MAJ_SPOT 2
#endif

#ifndef __cplusplus
uniform DirectionalLight u_dirLights[NR_DIR_LIGHTS];
uniform PointLight u_pointLights[NR_POINT_LIGHTS];
uniform SpotLight u_spotLights[NR_SPOT_LIGHTS];
#endif
