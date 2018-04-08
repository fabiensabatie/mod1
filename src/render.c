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
#define VERTEX "shaders/particles.vert"
#define FRAG_G "shaders/green.frag"
#define FRAG_B "shaders/blue.frag"

static GLuint createVAO(void)
{
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	return (vao);
}

// GLuint createBuffer(t_render *r)
// {
// 	GLuint vao;
// 	float res[176418];
//
// 	r->zmax = 0;
// 	int i = 0;
// 	for (float x = 0.1; x < 10; x += 0.1) {
// 		for (float y = 0; y < 9.9; y += 0.1) {
// 			res[i++] = x / 10 - 0.5;
// 			res[i++] = y / 10 - 0.5;
// 			res[i++] = -interpolation(r->size, r->points, x, y) / 10;
// 			r->zmax = (r->zmax > res[i - 1]) ? res[i - 1] : r->zmax;
// 			res[i++] = (x - 0.1) / 10 - 0.5;
// 			res[i++] = y / 10 - 0.5;
// 			res[i++] = -interpolation(r->size, r->points, x - 0.1, y) / 10;
// 			r->zmax = (r->zmax > res[i - 1]) ? res[i - 1] : r->zmax;
// 			res[i++] = (x - 0.1) / 10 - 0.5;
// 			res[i++] = (y + 0.1) / 10 - 0.5;
// 			res[i++] = -interpolation(r->size, r->points, x - 0.1, y + 0.1) / 10;
// 			r->zmax = (r->zmax > res[i - 1]) ? res[i - 1] : r->zmax;
// 			res[i++] = x / 10 - 0.5;
// 			res[i++] = y / 10 - 0.5;
// 			res[i++] = -interpolation(r->size, r->points, x, y) / 10;
// 			r->zmax = (r->zmax > res[i - 1]) ? res[i - 1] : r->zmax;
// 			res[i++] = x / 10 - 0.5;
// 			res[i++] = (y + 0.1) / 10 - 0.5;
// 			res[i++] = -interpolation(r->size, r->points, x, y + 0.1) / 10;
// 			r->zmax = (r->zmax > res[i - 1]) ? res[i - 1] : r->zmax;
// 			res[i++] = (x - 0.1) / 10 - 0.5;
// 			res[i++] = (y + 0.1) / 10 - 0.5;
// 			res[i++] = -interpolation(r->size, r->points, x - 0.1, y + 0.1) / 10;
// 			r->zmax = (r->zmax > res[i - 1]) ? res[i - 1] : r->zmax;
// 		}
// 	}
// 	vao = createVAO();
// 	GLuint vbo = 0;
// 	glGenBuffers(1, &vbo);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, 99 * 99 * 2 * 3 * 3 * sizeof(float), res, GL_DYNAMIC_DRAW);
// 	glEnableVertexAttribArray(0);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
// 	glEnableVertexAttribArray(0);
// 	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
// 	glEnable(GL_DEPTH_TEST);
// 	return (vao);
// }


GLuint createBuffer(t_render *r)
{
	GLuint vao;
	float res[r->part_number * 3];
	size_t i = 0;
	size_t y = 0;
	while (i < r->part_number * 3) {
		res[i++] = r->particles[y]->pos.x / 2000 - 0.5;
		res[i++] = r->particles[y]->pos.y / 2000 - 0.5;
		res[i++] = r->particles[y]->pos.z / 2000 - 0.5;
		y++;
	}

	vao = createVAO();
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, r->part_number * 3 * sizeof(float), res, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	return (vao);
}

void set_Uniforms(t_render *r, t_shader *vertex)
{
	float thetaz = 0;
	int rotYLoc = glGetUniformLocation(vertex->prog, "rot_Y");
	int txLoc = glGetUniformLocation(vertex->prog, "thetax");
	int tzLoc = glGetUniformLocation(vertex->prog, "thetaz");
	int zLoc = glGetUniformLocation(vertex->prog, "zmax");
	int psyLoc = glGetUniformLocation(vertex->prog, "psy");
	int tiLoc = glGetUniformLocation(vertex->prog, "t");
	float thetax = (M_PI / 4) * r->rotX / 20;
	float timeValue = glfwGetTime();
	float t = sin(timeValue) / 2.0f + 0.5f;
	t = (t < 0) ? -t : t;
	glUniform1f(txLoc, thetax);
	glUniform1f(tzLoc, thetaz);
	glUniform1f(zLoc, -0.05);
	glUniform1f(rotYLoc, r->rotY / 20);
	glUniform1i(psyLoc, r->psy);
	glUniform1f(tiLoc, t);
}

int	render(t_render *r)
{
	t_shader	*vertex;
	t_shader	*frag;
	GLuint		vao;

	r->draw_mod = GL_TRIANGLES;
	r->rotY = 0;
	r->rotX = 20;
	r->psy = 0;

	if (!(vertex = build_shader(VERTEX, GL_VERTEX_SHADER, 0, FALSE)))
		return (0);
	if (!(frag = build_shader(FRAG_G, GL_FRAGMENT_SHADER, vertex->prog, TRUE)))
		return (0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glfwSetWindowUserPointer(r->win, r);
	while (!glfwWindowShouldClose(r->win))
	{
		vao = createBuffer(r);

		updateParticlesState(r);
		glfwSetKeyCallback(r->win, event);
		set_Uniforms(r, vertex);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(vertex->prog);
		glBindVertexArray(vao);
		// glDrawArrays(r->draw_mod, 0, 58806);
		glDrawArrays(GL_POINTS, 0, r->part_number);
		glfwPollEvents();
		glfwSwapBuffers(r->win);
	}
	return (0);
}
