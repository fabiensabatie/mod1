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
	float vertices[18] = {-0.5f, -0.5f, 0.0, // Coin en bas à gauche
		 				-0.5f, 0.5f, 0.0, // Coin en haut a gauche
						0.5f, -0.5f, 0.0, // Coin en bas à droite
						/* Deuxieme triangle */
						-0.5f, 0.5f, 0.0, // Coin en haut a gauche
						0.5f, -0.5f, 0.0, // Coin en bas à droite
						0.5f, 0.5f, 0.0}; // Coin en haut à droite

	vao = createVAO();
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
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
		// Récupère l'heure
		float timeValue = glfwGetTime();
		// Définit la valeur de vert selon l'heure.
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		// Récupère l'id de la variable uniforme "ourColor" (voir shaders/green.frag)
		int vertexColorLocation = glGetUniformLocation(vertex->prog, "ourColor");
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(vertex->prog);
		// Assigne la valeur de green value à "ourColor"
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// update other events like input handling
		glfwPollEvents();
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(r->win);
	}
	return (0);
}
