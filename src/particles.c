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
const static float REST_DENS = 1100.f; // rest density
const static float GAS_CONST = 2000.f; // const for equation of state
const static float HP = 5.f; // kernel radius
const static float MASS = 65.f; // assume all particles have the same mass
const static float VISC = 250.f; // viscosity constant
const static float DT = 0.001f; // integration timestep

// simulation parameters
const static float BOUND_DAMPING = -0.5f;

t_pcl **initParticles(t_render *r)
{
	t_pcl **particles;
	int i = 0;
	float EPS = HP; // boundary epsilon


	// Ajouter un for ici pour initialiser le pos.z des particules.
	if (!(particles = (t_pcl**)malloc(sizeof(t_pcl*) * 2000000)))
		return (NULL);

	// for(float z = EPS; z <= 200-EPS*2.f; z += HP * 3)
		for(float y = EPS; y <= 200-EPS*2.f; y += HP * 3)
			for(float x = EPS; x <= 200-EPS*2.f; x += HP * 3) {
				if (!(particles[i] = (t_pcl*)malloc(sizeof(t_pcl))))
					return (NULL);
				particles[i]->v.x = 0.f;
				particles[i]->v.y = 0.f;
				particles[i]->pos.x = x + cos(x) * 3;
				particles[i++]->pos.y = y + cos(y) * 3;
				// particles[i++]->pos.z = z + cos(z);
			}

	r->part_number = i;
	r->particles = particles;
	return (particles);
}

static void computeDensityPressure(t_render *r, t_pcl **particles)
{
	float djix;
	float djiy;
	// float djiz;
	float resolver = 0.f;
	float poly;
	float HSQ = HP*HP; // radius^2 for optimization

	poly = 315.f/(65.f * M_PI * pow(HP, 9.f));
	printf("1 - Entered: %f %f\n", particles[13]->pos.x, particles[13]->pos.y);
	for (size_t i = 0; i < r->part_number; i++) {
		particles[i]->rho = 0.0f;
		for (size_t j = 0; j < r->part_number; j++)
		{
			djix = particles[j]->pos.x - particles[i]->pos.x;
			djiy = particles[j]->pos.y - particles[i]->pos.y;
			// djiz = particles[j]->pos.z - particles[i]->pos.z;
			resolver = powf(djix, 2) + powf(djiy, 2); //+ pow(djiz, 2);
			if (i == 13 && j == 0)
				printf("resolver1 : %f\n", resolver);
			if(resolver < HSQ)
			{
				// printf("MASS: %f POLY: %f POW: %f = %f\n", MASS,  poly, pow(HSQ - resolver, 2.f), MASS * poly * pow(HSQ - resolver, 2.f));
				// this computation is symmetric
				// printf("Rho: %7.f\n", particles[i]->rho);
				particles[i]->rho += MASS * poly * powf(HSQ - resolver, 2.f);
			}
		}
		if (i == 13)
			printf("rho %f\n", particles[i]->rho);
		particles[i]->p = GAS_CONST * (particles[i]->rho - REST_DENS);
	}
	printf("2Left DP with p: %f and rho %f\n", particles[13]->p, particles[13]->rho);
}

static void computeForces(t_render *r, t_pcl **particles)
{
	float djix;
	float djiy;
	// float djiz;
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
			// djiz = particles[j]->pos.z - particles[i]->pos.z;
			resolver = sqrtf(powf(djix, 2) + powf(djiy, 2)); // + pow(djiz, 2));
			if (i == 13 && j == 0)
				printf("3resolver 2: %f\n", resolver);
			if(resolver < HP && resolver != 0)
			{
				// compute pressure force contribution
				fpress.x += -djix / resolver * MASS * (particles[i]->p + particles[j]->p) /(2.f * particles[j]->rho) * sp * pow(HP - resolver ,2.f);
				fpress.y += -djiy / resolver * MASS * (particles[i]->p + particles[j]->p) /(2.f * particles[j]->rho) * sp * pow(HP - resolver ,2.f);
				// fpress.z += -djiz / resolver * MASS * (particles[i]->p + particles[j]->p) /(2.f * particles[j]->rho) * sp * pow(H - resolver ,2.f);
				// compute viscosity force contribution
				fvisc.x += VISC * MASS * (particles[j]->v.x - particles[i]->v.x) / particles[j]->rho * vl * (HP - resolver);
				fvisc.y += VISC * MASS * (particles[j]->v.y - particles[i]->v.y) / particles[j]->rho * vl * (HP - resolver);
				// fvisc.z += VISC * MASS * (particles[j]->v.z - particles[i]->v.z) / particles[j]->rho * vl * (H - resolver);
			}
		}
		fgrav.x = G[0] * particles[i]->rho;
		fgrav.y = G[1] * particles[i]->rho;
		if (i == 13) {
			printf("4GRAVITY: x %f y %f\n", fgrav.x, fgrav.y);
			printf("4VISC: x %f y %f\n", fvisc.x, fvisc.y);
			printf("4PRESS: x %f y %f\n", fpress.x, fpress.y);
		}
		// fgrav.z = G[1] * particles[i]->rho;
		if (i == 0)
			printf("f.x %f\n",fpress.x + fvisc.x + fgrav.x);
		particles[i]->f.x = fpress.x + fvisc.x + fgrav.x;
		particles[i]->f.y = fpress.y + fvisc.y + fgrav.y;
		// particles[i]->f.z = fpress.z + fvisc.z + fgrav.z;
		//printf("Forces on x: %f\nForces on y: %f\n", particles[i]->f.x, particles[i]->f.y);
	}
	printf("5Forces: x %f y %f\n", particles[13]->f.x, particles[13]->f.y);
}

static void integrate(t_render *r, t_pcl **particles)
{
	float EPS = HP; // boundary epsilon
	// static int i = 0;

	for(size_t i = 0; i < r->part_number; i++)
	{
		// forward Euler integration
		if (i == 0) {
			printf("Before assigning %f %f ", particles[13]->pos.x,  particles[13]->pos.y);
			printf("with DT %f and pcl: %f\n", DT, particles[i]->f.y);
		}
		particles[i]->v.x += DT * particles[i]->f.x / particles[i]->rho;
		particles[i]->v.y += DT * particles[i]->f.y / particles[i]->rho;
		// particles[i]->v.z += DT * particles[i]->f.z / particles[i]->rho;
		if (i == 13)
			printf("Before adding v with v %f %f\n", particles[13]->v.x,  particles[13]->v.y);
		particles[i]->pos.x += DT * particles[i]->v.x;
		particles[i]->pos.y += DT * particles[i]->v.y;
		// particles[i]->pos.z += DT * particles[i]->v.z;
		if (i == 13)
			printf("Before bound check with %f %f\n", particles[13]->pos.x,  particles[13]->pos.y);

		// enforce boundary conditions
		if(particles[i]->pos.x - EPS < 0.0f)
		{
			particles[i]->v.x  *= BOUND_DAMPING;
			particles[i]->pos.x  = EPS;
		}
		if(particles[i]->pos.x + EPS > 2000)
		{
			particles[i]->v.x  *= BOUND_DAMPING;
			particles[i]->pos.x  = 2000-EPS;
		}
		if(particles[i]->pos.y - EPS < 0.0f)
		{
			particles[i]->v.y *= BOUND_DAMPING;
			particles[i]->pos.y = EPS;
		}
		if(particles[i]->pos.y + EPS > 200)
		{
			particles[i]->v.y *= BOUND_DAMPING;
			particles[i]->pos.y = 2000-EPS;
		}
		// if(particles[i]->pos.z - EPS < 0.0f)
		// {
		// 	particles[i]->v.z *= BOUND_DAMPING;
		// 	particles[i]->pos.z = EPS;
		// }
		// // if(particles[i]->pos.z + EPS > 200)
		// // {
		// // 	particles[i]->v.z *= BOUND_DAMPING;
		// // 	particles[i]->pos.z = 2000-EPS;
		// // }
	}

	printf("Left with %f %f\n", particles[13]->pos.x,  particles[13]->pos.y);
}

void updateParticlesState(t_render *r)
{
	computeDensityPressure(r, r->particles);
	computeForces(r, r->particles);
	integrate(r, r->particles);
}
