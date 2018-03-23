#version 150

in vec4 vPosition;
in vec3 vNormal;

out vec3 posInCam;
out vec3 fN;
out vec3 fE;
out vec3 L_FlashLight;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

uniform vec4 sunLightDirection;

void main() 
{
	gl_Position = proj_matrix*view_matrix*model_matrix*vPosition;		// vertex position in world coordinates
	posInCam = (view_matrix*model_matrix*vPosition).xyz;				// vertex in camera coordinates
	fN = normalize((view_matrix*model_matrix*vec4(vNormal,0)).xyz);		// normal in camera coordinates
	fE = normalize(-posInCam);											// ray from vertex towards camera
	
	// flashlight
	L_FlashLight = normalize(-posInCam);					// ray from the vertex towards the light

	// sun
	//vec3 L_Sunlight = normalize(-sunLightDirection.xyz);	// for a directional light, we know L directly
}