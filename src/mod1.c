/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wwatkins <wwatkins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/29 17:23:49 by wwatkins          #+#    #+#             */
/*   Updated: 2016/12/12 11:34:21 by wwatkins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod1.h"

int	init_win(t_render *r)
{
	// Initialisation de la librairie et création de la fenêtre.
	if (!glfwInit())
		return (0);
	if (!(r->win = glfwCreateWindow(640, 480, "Hello World", NULL, NULL)))
	{
		glfwTerminate();
		return (0);
	}
	// Créé un contexte courant pour la fenêtre.
	glfwMakeContextCurrent(r->win);
	return (1);
}

int render(t_render *r)
{
	float vertices[] = {0.25, -0.25, -0,25 , 0.25, -0.25 , 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, -0.25,
						-0.25, -0.25, -0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25};

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(r->win))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// On remplie puis on active le tableau Vertex Attrib 0

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
		glEnableVertexAttribArray(0);
		// On affiche le triangle
		glDrawArrays(GL_LINES, 0, 8);
		// On désactive le tableau Vertex Attrib puisque l'on n'en a plus besoin
		glDisableVertexAttribArray(0);

		/* Swap front and back buffers */
		glfwSwapBuffers(r->win);

		/* Poll for and process events */
		glfwPollEvents();
	}
	return (0);
}

int main(void)
{
	t_render *r;

	if (!(r = (t_render*)malloc(sizeof(t_render))))
		exit(ft_printf("Could not malloc *r\n"));
	if (!init_win(r))
		exit(ft_printf("Could not initialize the window\n"));
		render(r);
	glfwTerminate();
}
