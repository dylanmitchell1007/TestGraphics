#version 410

in vec4 vNormal;
in vec4 vColor;
 out vec4 FragColor; 
 
 void main() 
 { 
	 FragColor = vec4(d * vColor.x, d * vColor.y, d * vColor.z, 1); 
	 float d = max(0, dot( normalize(vNormal.xyz), uniform vec3(0,1,0) ) ); 
 
 }