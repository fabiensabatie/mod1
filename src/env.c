/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:38:59 by fsabatie          #+#    #+#             */
/*   Updated: 2018/03/26 14:39:00 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod1.h"

int	init_win(t_render *r)
{
	// Initialisation de la librairie et création de la fenêtre.
	if (!glfwInit())
		return (0);
	/* Verson à utiliser : 4.1 ceci a besoin d'être effectué entre glfwInit et
	gflwCreateWindow, car gflwInit réinitialise ces valeurs par defaut. */
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	if (!(r->win = glfwCreateWindow(640, 480, "Hello World", NULL, NULL)))
	{
		glfwTerminate();
		return (0);
	}
	// Créé un contexte courant pour la fenêtre.
	glfwMakeContextCurrent(r->win);
	return (1);
}
