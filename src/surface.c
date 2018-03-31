/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:37:32 by fsabatie          #+#    #+#             */
/*   Updated: 2018/03/31 17:48:46 by vlay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod1.h"

#define X x
#define Y y

#include <stdio.h>
#include <stdlib.h>

float getZ(short n, float points[n][3],  float X, float Y) {
	float polynomes[n];
	float res = 0;
	float z;
	float zy;

	for (short i = 0; i < n; i++) {
		polynomes[i] = 1;
		for (short j = 0; j < n; j++) {
			if (j != i) {
				z = points[i][0] - points[j][0];
				zy = (points[i][1] - points[j][1]);
				z = (z == 0) ? 1 : z;
				zy = (zy == 0) ? 1 : zy;
				polynomes[i] *= (X - points[j][0]) / z * (Y - points[j][1]) / zy;
			}
		}
		polynomes[i] *= points[i][2];
	}
	for (short i = 0; i < n; i++) {
		res += polynomes[i];
	}
	return (res);
}

float interpolation(t_lagrange *lag, short n, float points[n][3], float X, float Y)
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
