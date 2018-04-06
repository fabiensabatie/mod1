#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
//layout (location = 1) in float theta;   // the position variable has attribute position 0
#define M_PI 3.1415926535897932384626433832795
uniform float theta;
out vec3 ourColor; // output a color to the fragment shader

void main()
{
	float zCos = cos(theta); //* rot;
	float zSin = sin(theta); //* rot;
	mat4 rotZ = mat4(zCos, zSin, 0, 0,
					-zSin, zCos, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);
	mat4 rotX = mat4(1, 0, 0, 0,
					0, zCos, zSin, 0,
					0, -zSin, zCos, 0,
					0, 0, 0, 1);
	mat4 rotY = mat4(zCos, 0, -zSin, 0,
					0, 1, 0, 0,
					zSin, 0, zCos, 0,
					0, 0, 0, 1);
	gl_Position = rotZ * vec4(aPos, 1.0);
	gl_Position = rotX * gl_Position;
	gl_Position = rotY * gl_Position;
	ourColor = vec3(1, 1, 1);
	// ourColor = vec3(1, 1, 1);
	gl_PointSize = 7.0; // set ourColor to the input color we got from the vertex data
}
