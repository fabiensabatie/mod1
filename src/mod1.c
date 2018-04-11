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
	if (ac == 2 && ft_strcmp(av[1], "GPU"))
	{
		if (!(parser(r, av[1]))
		|| !(initParticles(r))
		|| !(render(r)))
			return (0);
	}
	else if (!(ft_strcmp(av[1], "GPU"))) {
		if (!(r->k = (t_kernel*)build_kernel("rsc/kernels/particles.cl"))
		|| !(initParticles(r))
		|| !(render(r)))
			return (0);
	}
	else {
		printf("usage: ./mod1 [GPU] map.mod1\n");
	}
	glfwTerminate();
}
