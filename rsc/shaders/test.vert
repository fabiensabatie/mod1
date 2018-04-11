#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
//layout (location = 1) in float theta;   // the position variable has attribute position 0
#define M_PI 3.1415926535897932384626433832795
uniform float thetaz;
uniform float thetax;
uniform float rot_Y;
uniform float zmax;
uniform float t;
uniform int psy;
out vec3 ourColor; // output a color to the fragment shader

void main()
{
	float zCos = cos(thetaz); //* rot;
	float zSin = sin(thetaz); //* rot;
	float xCos = cos(thetax); //* rot;
	float xSin = sin(thetax); //* rot;
	float yCos = cos(M_PI / 4 * rot_Y); //* rot;
	float ySin = sin(M_PI / 4 * rot_Y); //* rot;
	float VR = 102 - 50;
	float VG = 255 - 150;
	float VB = 51 - 150;
	float r = 0;
	float g = 0;
	float b = 0;
	mat4 rotZ = mat4(zCos, zSin, 0, 0,
					-zSin, zCos, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);

	mat4 rotX = mat4(	1, 0, 0, 0,
						0, xCos, xSin, 0,
						0, -xSin, xCos, 0,
						0, 0, 0, 1);

	mat4 rotY  = mat4(	yCos, 0, -ySin, 0,
						0, 1, 0, 0,
						ySin, 0, yCos, 0,
						0, 0, 0, 1);

	gl_Position = vec4(aPos, 1.0);
	gl_Position = rotZ * vec4(aPos, 1.0);
	gl_Position = rotX * gl_Position;
	gl_Position = rotY * gl_Position;

	if (psy % 2 == 0) {
		r = 50.0/255.0 + (VR * (aPos.z / zmax) * 0.002);
		g = 150/255 + (VG * (aPos.z / zmax) * 0.002);
		b = 150/255 + (VB * (aPos.z / zmax) * 0.002);
	}
	else {
		r = 1 - (gl_Position.y + 0.6) + xCos * t * 0.2;
		g = 1 - (-gl_Position.z + 0.6) + yCos * t * 0.2;
		b = 1 - (gl_Position.x + 0.6)+ zSin * t * 0.2;
	}
	ourColor = vec3(r, g, b);
	// ourColor = vec3(1, 1, 1);
	gl_PointSize = 1; // set ourColor to the input color we got from the vertex data
}
