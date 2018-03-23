#version 150

in vec3 posInCam;
in vec3 fN;
in vec3 fE;
in vec3 L_FlashLight;

out vec4 fColor;

// materials
uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;

// lighting
uniform vec4 flashLightAmbient, flashLightDiffuse, flashLightSpecular;
uniform int flashLightEnabled;
uniform float flashLightAngle;
uniform vec4 sunLightAmbient, sunLightDiffuse, sunLightSpecular, sunLightDirection;
uniform int sunLightEnabled;


void main() 
{ 
    vec3 N = normalize(fN);
	vec3 E = normalize(fE);

	// flashlight
	vec4 flashLightColor = vec4(0,0,0,0);
	if(flashLightEnabled == 1)
	{
		vec3 L = normalize(L_FlashLight);
		vec3 H = normalize(L+E);

		// ambient
		vec4 ambient = flashLightAmbient*matAmbient;
		
		// diffuse
		float Kd = max(dot(L,N),0.0);
		vec4 diffuse = Kd*flashLightDiffuse*matDiffuse;
		
		// specular
		float Ks = pow(max(dot(N,H),0.0),matAlpha);
		vec4 specular = Ks*flashLightSpecular*matSpecular;
		if(dot(L,N)<0.0)
			specular = vec4(0,0,0,1);

		// spotlight properties
		float distFactor = 1/pow(length(posInCam),0.8);
		float maxAngle = radians(flashLightAngle);
		float angle = acos(dot(vec3(0,0,-1), -L));
	
		if(angle>maxAngle) 
		{
			flashLightColor = vec4(0,0,0,1);
		}
		else
		{
			flashLightColor = pow(max(dot(vec3(0,0,-1),-L),0),100)*distFactor*(ambient + diffuse + specular);
		}
	}


	// sun
	vec4 sunLightColor = vec4(0,0,0,0);
	if(sunLightEnabled == 1)
	{
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
	}


	// resulting color
	fColor = flashLightColor + sunLightColor;
	fColor.a = 1.0;
} 

