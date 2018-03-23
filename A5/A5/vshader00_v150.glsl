#version 150

in vec4 vPosition;
in vec3 vNormal;

out vec4 color;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

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
  	gl_Position = proj_matrix*view_matrix*model_matrix*vPosition;			// vertex position in world coordinates
	vec3 vertexPosInCam = (view_matrix*model_matrix*vPosition).xyz;			// vertex in camera coordinates
	vec3 N = normalize((view_matrix*model_matrix*vec4(vNormal,0)).xyz);		// normal in camera coordinates
	vec3 V = normalize(-vertexPosInCam);									// ray from vertex towards camera

	// flashlight
	vec4 flashLightColor = vec4(0,0,0,0);
	if(flashLightEnabled == 1)
	{
		// flashlight will always be (0,0,0) in cam coordinates, this simplifies some things
		vec3 L = normalize(-vertexPosInCam);  	// ray from vertex towards light
		vec3 H = normalize(L+V);
		
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
		float distFactor = 1/pow(length(vertexPosInCam),0.8);
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
		vec3 H = normalize(L+V);

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
	color = flashLightColor + sunLightColor;
	color.a = 1.0;
}