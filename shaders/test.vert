#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
//layout (location = 1) in float theta;   // the position variable has attribute position 0

uniform float theta;
uniform float rot;
out vec3 ourColor; // output a color to the fragment shader
void main()
{
	float aCos = cos(theta); //* rot;
	float aSin = sin(theta); //* rot;
	float yCos = cos(theta * rot); //* rot;
	float ySin = sin(theta * rot); //* rot;

	mat4 rotZ = mat4(aCos, aSin, 0, 0,
					-aSin, aCos, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);

	mat4 rotX = mat4(	1, 0, 0, 0,
						0, aCos, aSin, 0,
						0, -aSin, aCos, 0,
						0, 0, 0, 1);

	mat4 rotY  = mat4(	yCos, 0, -ySin, 0,
						0, 1, 0, 0,
						ySin, 0, yCos, 0,
						0, 0, 0, 1);
	gl_Position = vec4(aPos, 1.0);
	ourColor = vec3(1.0 - gl_Position.x * 2, 1.0 - gl_Position.y *2, 1.0 - gl_Position.z * 2);
	gl_Position = rotZ * vec4(aPos, 1.0);
	gl_Position = rotX * gl_Position;
	gl_Position = rotY * gl_Position;
	gl_PointSize = 1.0; // set ourColor to the input color we got from the vertex data
}
