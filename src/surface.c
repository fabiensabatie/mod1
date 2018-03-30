/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:37:32 by fsabatie          #+#    #+#             */
/*   Updated: 2018/03/26 14:37:33 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "mod1.h"
#define X x
#define Y y

#include <stdio.h>


float getZ(float points[5][3], short n, float X, float Y) {
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
