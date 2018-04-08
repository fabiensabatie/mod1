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
// solver parameters
const float G[2] = {0.f, 12000*-9.8f}; // external (gravitational) forces
const static float REST_DENS = 200.f; // rest density
const static float GAS_CONST = 500.f; // const for equation of state
const static float HP = 80.f; // kernel radius
const static float MASS = 20.f; // assume all particles have the same mass
const static float VISC = 100.f; // viscosity constant
const static float DT = 0.004f; // integration timestep

// simulation parameters
const static float BOUND_DAMPING = -0.0f;

t_pcl **initParticles(t_render *r)
{
	t_pcl **particles;
	int i = 0;
	float EPS = HP; // boundary epsilon

	if (!(particles = (t_pcl**)malloc(sizeof(t_pcl*) * 2000000)))
		return (NULL);
	for(float z = EPS; z <= 10000-EPS*2.f; z += HP * 2)
		for(float y = EPS; y <= 1000-EPS*2.f; y += HP * 3)
			for(float x = EPS; x <= 1000-EPS*2.f; x += HP * 3) {
				if (!(particles[i] = (t_pcl*)malloc(sizeof(t_pcl))))
					return (NULL);
				particles[i]->v.x = 0.f;
				particles[i]->v.y = 0.f;
				particles[i]->v.z = 0.f;
				particles[i]->pos.x = x + (float)rand() / (float)RAND_MAX;
				particles[i]->pos.y = y + (float)rand() / (float)RAND_MAX;
				particles[i++]->pos.z = z + (float)rand() / (float)RAND_MAX;
			}

	r->part_number = i;
	r->particles = particles;
	return (particles);
}

static void computeDensityPressure(t_render *r, t_pcl **particles)
{
	float djix;
	float djiy;
	float djiz;
	float resolver = 0.f;
	float poly;
	float HSQ = HP*HP*50; // radius^2 for optimization

	poly = 315.f/(65.f * M_PI * pow(HP, 9.f));
	for (size_t i = 0; i < r->part_number; i++) {
		particles[i]->rho = 0.0f;
		for (size_t j = 0; j < r->part_number; j++)
		{
			djix = particles[j]->pos.x - particles[i]->pos.x;
			djiy = particles[j]->pos.y - particles[i]->pos.y;
			djiz = particles[j]->pos.z - particles[i]->pos.z;
			resolver = powf(djix, 2) + powf(djiy, 2) + powf(djiz, 2); //+ pow(djiz, 2);
			if(resolver < HSQ)
				particles[i]->rho += MASS * poly * powf(HSQ - resolver, 2.f);
		}
		particles[i]->p = GAS_CONST * (particles[i]->rho - REST_DENS);
	}
}

static void computeForces(t_render *r, t_pcl **particles)
{
	float djix;
	float djiy;
	float djiz;
	float resolver;
	float sp;
	float vl;
	vec3D fgrav;

	sp = -45.f / (M_PI * powf(HP, 6.f));
	vl = 45.f / (M_PI * powf(HP, 6.f));
	for (size_t i = 0; i < r->part_number; i++) {
		vec3D fpress = {0.f, 0.f, 0.f};
		vec3D fvisc = {0.f, 0.f, 0.f};
		for (size_t j = 0; j < r->part_number; j++)
		{
			if(i == j)
				continue ;
			djix = particles[j]->pos.x - particles[i]->pos.x;
			djiy = particles[j]->pos.y - particles[i]->pos.y;
			djiz = particles[j]->pos.z - particles[i]->pos.z;
			resolver = sqrtf(powf(djix, 2) + powf(djiy, 2) + pow(djiz, 2));
			if(resolver < HP && resolver != 0)
			{
				fpress.x += -djix / resolver * MASS * (particles[i]->p + particles[j]->p) /(2.f * particles[j]->rho) * sp * pow(HP - resolver ,2.f);
				fpress.y += -djiy / resolver * MASS * (particles[i]->p + particles[j]->p) /(2.f * particles[j]->rho) * sp * pow(HP - resolver ,2.f);
				fpress.z += -djiz / resolver * MASS * (particles[i]->p + particles[j]->p) /(2.f * particles[j]->rho) * sp * pow(H - resolver ,2.f);
				fvisc.x += VISC * MASS * (particles[j]->v.x - particles[i]->v.x) / particles[j]->rho * vl * (HP - resolver);
				fvisc.y += VISC * MASS * (particles[j]->v.y - particles[i]->v.y) / particles[j]->rho * vl * (HP - resolver);
				fvisc.z += VISC * MASS * (particles[j]->v.z - particles[i]->v.z) / particles[j]->rho * vl * (H - resolver);
			}
		}
		fgrav.x = G[0] * particles[i]->rho;
		fgrav.y = G[0] * particles[i]->rho;
		fgrav.z = G[1] * particles[i]->rho;
		particles[i]->f.x = fpress.x + fvisc.x + fgrav.x;
		particles[i]->f.y = fpress.y + fvisc.y + fgrav.y;
		particles[i]->f.z = fpress.z + fvisc.z + fgrav.z;
	}
}

static void integrate(t_render *r, t_pcl **particles)
{
	float EPS = HP; // boundary epsilon
	for(size_t i = 0; i < r->part_number; i++)
	{

		particles[i]->v.x += DT * particles[i]->f.x / particles[i]->rho;
		particles[i]->v.y += DT * particles[i]->f.y / particles[i]->rho;
		particles[i]->v.z += DT * particles[i]->f.z / particles[i]->rho;
		particles[i]->pos.x += DT * particles[i]->v.x;
		particles[i]->pos.y += DT * particles[i]->v.y;
		particles[i]->pos.z += DT * particles[i]->v.z;

		// enforce boundary conditions
		if(particles[i]->pos.x - EPS <= 0.0f)
		{
			particles[i]->v.x  *= BOUND_DAMPING;
			particles[i]->pos.x  = EPS + (float)rand() / (float)RAND_MAX;
		}
		if(particles[i]->pos.x + EPS >= 1000)
		{
			particles[i]->v.x  *= BOUND_DAMPING;
			particles[i]->pos.x  = 1000 - EPS - (float)rand() / (float)RAND_MAX;
		}
		if(particles[i]->pos.y - EPS <= 0.0f)
		{
			particles[i]->v.y *= BOUND_DAMPING;
			particles[i]->pos.y = EPS + (float)rand() / (float)RAND_MAX;
		}
		if(particles[i]->pos.y + EPS >= 1000)
		{
			particles[i]->v.y *= BOUND_DAMPING;
			particles[i]->pos.y = 1000 - EPS - (float)rand() / (float)RAND_MAX;
		}
		if(particles[i]->pos.z - EPS < 0.0f)
		{
			particles[i]->v.z *= BOUND_DAMPING;
			particles[i]->pos.z = EPS + (float)rand() / (float)RAND_MAX;
		}
		if(particles[i]->pos.z + EPS > 1000)
		{
			particles[i]->v.z *= BOUND_DAMPING;
			particles[i]->pos.z = 1000 - EPS - (float)rand() / (float)RAND_MAX;
		}
	}
}

void updateParticlesState(t_render *r)
{
	computeDensityPressure(r, r->particles);
	computeForces(r, r->particles);
	integrate(r, r->particles);
}
