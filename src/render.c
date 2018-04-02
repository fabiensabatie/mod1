/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:39:41 by fsabatie          #+#    #+#             */
/*   Updated: 2018/04/01 20:24:50 by vlay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod1.h"
#define VERTEX "shaders/test.vert"
#define FRAG_G "shaders/green.frag"
#define FRAG_B "shaders/blue.frag"


static GLuint createVAO(void)
{
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	return (vao);
}


static GLuint createBuffer(void)
{
	GLuint vao;

	int pt = 21;
	float points[21][3] = {{5000,5000,5000},
	{5000,15000,5000},
	{15000,15000,4000},
	{15000,5000,5000},
	{7400,10000,3000},
	{7400,15000,3000},
	{12600,15000,3000},
	{12600,10000,3000},
	{5000,10000,5000},
	{15000,10000,5000},
	{10000,15000,2000},
	{10000,12600,1000},
	{10000,10000,0},
	{10000,7400,0},
	{6200,10000,0},
	{13800,10000,0},
	{5000,7400,5000},
	{15000,7400,5000},
	{5000,12600,5000},
	{15000,12600,4500},
	{10000,5000,5000}};

	for	(int i = 0; i < 21 ; i++){
		for	(int y = 0; y < 3 ; y++){
			points[i][y] /= (float)2000;
		}
	}


	float res[176418];
	int i = 0;
	int t = 0;
	for (float x = 0.1; x < 10; x += 0.1) {
		for (float y = 0; y < 9.9; y += 0.1) {
			res[i++] = x / 10 - 0.5;
			res[i++] = y / 10 - 0.5;
			res[i++] = -interpolation(pt, points, x, y) / 10;
			res[i++] = (x - 0.1) / 10 - 0.5;
			res[i++] = y / 10 - 0.5;
			res[i++] = -interpolation(pt, points, x - 0.1, y) / 10;
			res[i++] = (x - 0.1) / 10 - 0.5;
			res[i++] = (y + 0.1) / 10 - 0.5;
			res[i++] = -interpolation(pt, points, x - 0.1, y + 0.1) / 10;
			t++;
			
			res[i++] = x / 10 - 0.5;
			res[i++] = y / 10 - 0.5;
			res[i++] = -interpolation(pt, points, x, y) / 10;
			res[i++] = x / 10 - 0.5;
			res[i++] = (y + 0.1) / 10 - 0.5;
			res[i++] = -interpolation(pt, points, x, y + 0.1) / 10;
			res[i++] = (x - 0.1) / 10 - 0.5;
			res[i++] = (y + 0.1) / 10 - 0.5;
			res[i++] = -interpolation(pt, points, x - 0.1, y + 0.1) / 10;
			t++;
			
		}
	}
	ft_printf("Got %d triangles\n", t);
	vao = createVAO();
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 99 * 99 * 2 * 3 * 3 * sizeof(float), res, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	return (vao);
}


int	render(t_render *r)
{
	t_shader	*vertex;
	t_shader	*frag;
	GLuint		vao;
	float		rot = 0;

	if (!(vertex = build_shader(VERTEX, GL_VERTEX_SHADER, 0, FALSE)))
		return (0);
	if (!(frag = build_shader(FRAG_G, GL_FRAGMENT_SHADER, vertex->prog, TRUE)))
		return (0);
	vao = createBuffer();

	int rLoc = glGetUniformLocation(vertex->prog, "rot");
	int tLoc = glGetUniformLocation(vertex->prog, "theta");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(r->win))
	{
		// // Récupère l'id de la variable uniforme "ourColor" (voir shaders/green.frag)
		float theta = M_PI / 4;
		rot++;
		glUniform1f(tLoc, theta);
		glUniform1f(rLoc, rot / 20);
		// Fond blanc assigné à GL_COLOR_BUFFER_BIT
		// Clear la fenêtre
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Utilise le programme de shader
		glUseProgram(vertex->prog);
		// Assigne la valeur de green value à "ourColor"
		// Lie le vao
		glBindVertexArray(vao);
		// Dessine les points (6 vertices ici)
		glDrawArrays(GL_LINES, 0, 58806);
		// Récupère les events
		glfwPollEvents();
		/* Swap le buffer (OpenGL utilise un buffer pour dessiner, l'affiche,
		et commence à dessiner dans celui qui était précedemment affiché */
		glfwSwapBuffers(r->win);
	}
	return (0);
}
