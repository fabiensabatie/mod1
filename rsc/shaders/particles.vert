#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
//layout (location = 1) in float theta;   // the position variable has attribute position 0
#define M_PI 3.1415926535897932384626433832795
uniform float theta;
out vec3 ourColor; // output a color to the fragment shader

void main()
{
	float zCos = cos(M_PI / 4); //* rot;
	float zSin = sin(M_PI / 4); //* rot;
	float yCos = cos(M_PI / 4); //* rot;
	float ySin = sin(M_PI / 4); //* rot;
	float xCos = cos(M_PI / -4); //* rot;
	float xSin = sin(M_PI / -4); //* rot;
	mat4 rotZ = mat4(zCos, zSin, 0, 0,
					-zSin, zCos, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);
	mat4 rotX = mat4(1, 0, 0, 0,
					0, xCos, xSin, 0,
					0, -xSin, xCos, 0,
					0, 0, 0, 1);
	mat4 rotY = mat4(yCos, 0, -ySin, 0,
					0, 1, 0, 0,
					ySin, 0, yCos, 0,
					0, 0, 0, 1);

	gl_Position = rotZ * vec4(aPos, 1.0);
	gl_Position = rotX * gl_Position;
	ourColor = vec3(gl_Position.x + 0.5, gl_Position.y + 0.5, gl_Position.z + 0.5);
	gl_PointSize = 3.0; // set ourColor to the input color we got from the vertex data
}
