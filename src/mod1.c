/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:37:11 by fsabatie          #+#    #+#             */
/*   Updated: 2018/04/01 18:58:20 by vlay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod1.h"

int main(int ac, char **av)
{
	t_render *r;

	if (!(r = (t_render*)malloc(sizeof(t_render))))
		exit(ft_printf("Could not malloc *r\n"));
	if (!init_win(r))
		exit(ft_printf("Could not initialize the window\n"));
	if (ac == 2)
	{
		if (!(parser(r, av[1])))
			return (0);
		if (!(initParticles(r)))
			return (0);
		if (!render(r))
			return (0);
	}
	else
		ft_printf("usage: mod1 [-obj] [file]");

	glfwTerminate();
}
