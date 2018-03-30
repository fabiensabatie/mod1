#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
out vec3 ourColor; // output a color to the fragment shader
uniform double aCos;
uniform double aSin;

mat4 rotX(vec4 vertex)
{
	mat4 rotx = mat4(
		1, 0, 0, 0,
		0, aCos, aSin, 0,
		0, aSin, aCos, 0,
		0, 0, 0, 1
		);
	vertex = rotX * vertex;
	return (vertex);
}

void main()
{
	gl_Position = rotX(vec4(aPos, 1.0));
	ourColor = vec3(1.0, 0.0, 0.0); // set ourColor to the input color we got from the vertex data
}
