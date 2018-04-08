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

void	event(GLFWwindow* window, int key, int scancode, int action, \
		int mods)
{
	t_render *r = glfwGetWindowUserPointer(window);
	(void)scancode;
	(void)mods;
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		if (r->draw_mod == GL_TRIANGLES)
			r->draw_mod = GL_POINTS;
		else if (r->draw_mod == GL_POINTS)
			r->draw_mod = GL_LINE_STRIP;
		else if (r->draw_mod == GL_LINE_STRIP)
			r->draw_mod = GL_TRIANGLES;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
		r->rotY++;
	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
		r->rotY--;
	if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
		r->rotX--;
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
		r->rotX++;
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		r->psy++;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		initParticles(r);
	}
}
