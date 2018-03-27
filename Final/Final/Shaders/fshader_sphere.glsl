#version 150

in vec3 posInCam;
in vec3 texCoord;
in vec3 fN;
in vec3 fE;

out vec4 fColor;

// materials
uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;
uniform samplerCube cubeMap;

// lighting
uniform vec4 sunLightAmbient, sunLightDiffuse, sunLightSpecular, sunLightDirection;


void main() 
{ 
    vec3 N = normalize(fN);
	vec3 E = normalize(fE);

	// sun
	vec4 sunLightColor = vec4(0,0,0,0);
	
	vec3 L = -sunLightDirection.xyz;	// for a directional light, we know L directly
	vec3 H = normalize(L+E);

	// ambient
	vec4 ambient = sunLightAmbient*matAmbient;
		
	// diffuse
	float Kd = max(dot(L,N),0.0);
	vec4 diffuse = Kd*sunLightDiffuse*matDiffuse;
	
	// specular
	float Ks = pow(max(dot(N,H),0.0),matAlpha);
	vec4 specular = Ks*sunLightSpecular*matSpecular;
	if(dot(L,N)<0.0)
		specular = vec4(0,0,0,1);

	sunLightColor = (ambient + diffuse + specular);


	// resulting color
	fColor = sunLightColor * texture(cubeMap, texCoord);
	fColor.a = 1.0;
} 

