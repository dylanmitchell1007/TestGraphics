#version 410
in vec4 vNormal; 
in vec4 vPosition;
in vec4 vColor; 

uniform vec3 lightDirection;
uniform vec3 lightColor;
out vec4 FragColor; 
 
 void main() 
 { 
   vec3 L = normalize(lightDirection.xyz);
   vec3 N = normalize(vNormal.xyz);
	float lambert = max(0, dot(N, L));
    vec3 materialColor = vec3(1);
	vec3 Diffuse = lightColor * materialColor* lambert;
//	float d = max(0, dot( normalize(vNormal.xyz), vec3(0,1,0) ) );
	
	FragColor = vec4( Diffuse, 1);
 }