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

# define HP 100.f // kernel radius

t_pcl *initParticles(t_render *r)
{
	t_pcl *particles;
	int i = 0;
	float EPS = HP; // boundary epsilon

	if (!(particles = (t_pcl*)malloc(sizeof(t_pcl) * 200000)))
		return (NULL);
	for(float z = EPS; z <= 2000-EPS*2.f; z += HP * 2)
		for(float y = EPS; y <= 2000-EPS*2.f; y += HP)
			for(float x = EPS; x <= 2000-EPS*2.f; x += HP) {
				particles[i].rho = 0;
				particles[i].p = 0;
				particles[i].fx = 0;
				particles[i].fy = 0;
				particles[i].fz = 0;
				particles[i].group = 0;
				particles[i].vx = 0.f;
				particles[i].vy = 0.f;
				particles[i].vz = 0.f;
				particles[i].posx = x + (float)rand() / (float)RAND_MAX;
				particles[i].posy = y + (float)rand() / (float)RAND_MAX;
				particles[i++].posz = z + (float)rand() / (float)RAND_MAX;
			}
	r->part_number = i;
	// printf("Got %li particles\n", r->part_number);
	r->particles = particles;
	return (particles);
}
