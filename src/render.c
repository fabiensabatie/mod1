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
	t_lagrange lag;
<<<<<<< HEAD
	int pt = 9;
	float points[9][3] = {{5, 5, 5},
						{2, 3, 3},
						{4, 1, 2},
						{3, 5, 1},
						{1, 1, 5},
						{7, 8, 10},
						{0, 8, 3},
						{7, 2, 6},
						{6, 9, 5}
						};
	float res[30000];
	int i = 0;
	for (float x = 0; x < 10; x += 0.1) {
		for (float y = 0; y < 10; y += 0.1) {
			res[i++] = x / 10 - 0.5;
			res[i++] = y / 10 - 0.5;
			res[i++] = -interpolation(&lag, pt, points, x, y) / 10;
			printf("interpol: %f %f %f\n", res[i - 3], res[i - 2], res[i - 1]);
=======
	float	points[6][3] ={{0, 0, 0},
						{20000, 20000, 0},
						{10000, 10000, 5000},
						{5000, 5000, 5000},
						{20000, 0, 0},
						{0, 20000, 0}};

	diffx(6, points, &lag);
	diffy(6, points, &lag);
	float res[1200000];
	int i = 0;
	for (float x = 0; x < 20000; x += 100) {
		for (float y = 0; y < 20000; y += 100) {
			res[i++] = x / 20000 - 0.5;
			res[i++] = y / 20000 - 0.5;
			res[i++] = -interpolation(&lag, 6, points, x, y) / 20000;
			// printf("%f %f %f\n", res[i - 3], res[i - 2], res[i - 1]);
>>>>>>> 7ab33adfb3595d231282e888e353a2af7af06728
		}
	}

	vao = createVAO();
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 30000 * sizeof(float), res, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
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

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(r->win))
	{
		// // Récupère l'id de la variable uniforme "ourColor" (voir shaders/green.frag)
		float theta = M_PI / 4;
		rot++;
		glUniform1f(tLoc, theta);
		glUniform1f(rLoc, rot / 20);
		// Fond blanc assigné à GL_COLOR_BUFFER_BIT
		glClearColor(0.0, 0.0, 0.0, 0.0);
		// Clear la fenêtre
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Utilise le programme de shader
		glUseProgram(vertex->prog);
		// Assigne la valeur de green value à "ourColor"
		// Lie le vao
		glBindVertexArray(vao);
		// Dessine les points (6 vertices ici)
		glDrawArrays(GL_POINTS, 0, 10000);
		// Récupère les events
		glfwPollEvents();
		/* Swap le buffer (OpenGL utilise un buffer pour dessiner, l'affiche,
		et commence à dessiner dans celui qui était précedemment affiché */
		glfwSwapBuffers(r->win);
	}
	return (0);
}
