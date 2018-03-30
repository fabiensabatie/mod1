#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
//layout (location = 1) in float theta;   // the position variable has attribute position 0

uniform float theta;
//uniform float rot;
out vec3 ourColor; // output a color to the fragment shader

void main()
{
	float aCos = cos(theta); //* rot;
	float aSin = sin(theta); //* rot;

	mat4 rotZ = mat4(aCos, aSin, 0, 0,
					-aSin, aCos, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1);

	mat4 rotX = mat4(	1, 0, 0, 0,
						0, aCos, aSin, 0,
						0, -aSin, aCos, 0,
						0, 0, 0, 1);

	mat4 rotY  = mat4(	aCos, 0, -aSin, 0,
						0, 1, 0, 0,
						aSin, 0, aCos, 0,
						0, 0, 0, 1);
	// gl_Position = vec4(aPos, 1.0);
	gl_Position = rotZ * vec4(aPos, 1.0);
	gl_Position = rotX * gl_Position;

	ourColor = vec3(1.0, 1.0, 1.0); // set ourColor to the input color we got from the vertex data
}
