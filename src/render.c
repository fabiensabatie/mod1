/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:39:41 by fsabatie          #+#    #+#             */
/*   Updated: 2018/03/26 14:39:43 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod1.h"
#include <math.h>
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

	float points[12][3] ={{0, 0, 0},
						{20000, 20000, 0},
						{10000, 2000, 10000},
						{20000, 0, 0},
						{0, 20000, 0}};

	float res[1200];
	int i = 0;
	for (float x = 0; x < 20000; x += 1000) {
		for (float y = 0; y < 20000; y += 1000) {
			res[i++] = x / 20000 - 0.5;
			res[i++] = y / 20000 - 0.5;
			res[i++] = -getZ(12, points, x, y) / 20000;
			printf("%f %f %f\n", res[i - 3], res[i - 2], res[i - 1]);
		}
	}

	float tmp;

	for (size_t i = 1; i < 1200; i += 3) {
		for (size_t y = 1; y < 1200; y += 3) {
			if (res[i] < res[y]) {
				tmp = res[i];
				res[i] = res[y];
				res[y] = tmp;
				tmp = res[i - 1];
				res[i - 1] = res[y - 1];
				res[y - 1] = tmp;
				tmp = res[i + 1];
				res[i + 1] = res[y + 1];
				res[y + 1] = tmp;
			}
		}
	}
	for (int x = 0; x < 1200; x += 3) {
			printf("%f %f %f\n", res[x], res[x + 1], res[x + 2]);
	}

	vao = createVAO();
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 1200 * sizeof(float), res, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	return (vao);
}


int	render(t_render *r)
{
	t_shader	*vertex;
	t_shader	*frag;
	GLuint		vao;

	if (!(vertex = build_shader(VERTEX, GL_VERTEX_SHADER, 0, FALSE)))
		return (0);
	if (!(frag = build_shader(FRAG_G, GL_FRAGMENT_SHADER, vertex->prog, TRUE)))
		return (0);
	vao = createBuffer();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(r->win))
	{
		// // Récupère l'id de la variable uniforme "ourColor" (voir shaders/green.frag)
		float rot = M_PI / 4;
		int rotLoc = glGetUniformLocation(vertex->prog, "theta");
		glUniform1f(rotLoc, rot);
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
		glDrawArrays(GL_LINE_STRIP, 0, 400);
		// Récupère les events
		glfwPollEvents();
		/* Swap le buffer (OpenGL utilise un buffer pour dessiner, l'affiche,
		et commence à dessiner dans celui qui était précedemment affiché */
		glfwSwapBuffers(r->win);
	}
	return (0);
}
