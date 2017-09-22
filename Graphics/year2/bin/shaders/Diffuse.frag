#version 410

in vec4 vNormal;
in vec4 vColor;
out vec4 FragColor; 
 
 void main() 
 { 
	float d = max(0, dot( normalize(vNormal.xyz), vec3(0,1,0) ) ); 
	FragColor = vec4(d * vColor.x, d * vColor.y, d * vColor.z, 1);
 }