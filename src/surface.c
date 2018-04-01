/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:37:32 by fsabatie          #+#    #+#             */
/*   Updated: 2018/04/01 20:24:20 by vlay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod1.h"

#define X x
#define Y y

#include <stdio.h>
#include <stdlib.h>

float	interpolation(t_lagrange *lag, short n, float points[n][3], float X, float Y)
{
	short	i = 0;
	float	polynome = 0;

	while (i < n)
	{
		polynome += points[i][2] * (Lx(lag, points[i][0], X) * Ly(lag, points[i][1], Y));
		i++;
	}
	return (polynome);
}
