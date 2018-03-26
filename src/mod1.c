/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:37:11 by fsabatie          #+#    #+#             */
/*   Updated: 2018/03/26 14:37:21 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod1.h"

int main(void)
{
	t_render *r;

	if (!(r = (t_render*)malloc(sizeof(t_render))))
		exit(ft_printf("Could not malloc *r\n"));
	if (!init_win(r))
		exit(ft_printf("Could not initialize the window\n"));
	if (!render(r))
		return (0);
	glfwTerminate();
}
