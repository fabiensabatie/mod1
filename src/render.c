/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:39:41 by fsabatie          #+#    #+#             */
/*   Updated: 2018/03/31 18:05:00 by vlay             ###   ########.fr       */
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
	t_lagrange lag;
	float points[16][3] ={{0, 0, 0},
						{20000, 20000, 0},
						{4000,4000,2000},
						{8000,4000,0},
						{12000,4000,2000},
						{16000,4000,200},
						{4000,8000,500},
						{8000,8000,2000},
						{12000,8000,200},
						{16000,8000,2000},
						{4000,12000,2000},
						{8000,12000,0},
						{12000,12000,2000},
						{16000,12000,200},
						{20000, 0, 0},
						{0, 20000, 0}};

	diffx(5, points, &lag);
	diffy(5, points, &lag);
	float res[1200000];
	int i = 0;
	for (float x = 0; x < 20000; x += 100) {
		for (float y = 0; y < 20000; y += 100) {
			res[i++] = x / 20000 - 0.5;
			res[i++] = y / 20000 - 0.5;
			res[i++] = -interpolation(&lag, 16, points, x, y) / 20000;
			// printf("%f %f %f\n", res[i - 3], res[i - 2], res[i - 1]);
		}
	}

	vao = createVAO();
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 120000 * sizeof(float), res, GL_DYNAMIC_DRAW);
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
		glDrawArrays(GL_POINTS, 0, 40000);
		// Récupère les events
		glfwPollEvents();
		/* Swap le buffer (OpenGL utilise un buffer pour dessiner, l'affiche,
		et commence à dessiner dans celui qui était précedemment affiché */
		glfwSwapBuffers(r->win);
	}
	return (0);
}
