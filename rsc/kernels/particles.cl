typedef struct	s_vec3D {

	float		x;
	float		y;
	float		z;
}				t_vect;

typedef struct	s_pcl {
	float		posx; // Position
	float		posy;
	float		posz;
	float		vx; // Velocity
	float		vy; // Velocity
	float		vz; // Velocity
	float		fx; // Velocity
	float		fy; // Velocity
	float		fz; // Velocity
	float		rho; // Density
	float		p; // Pressure
	int			group;
}				t_pcl;

typedef struct	s_grp
{
	size_t		groups;
	size_t		pcls;
	int			coord[3];
	int			pcl[1000];
}				t_grp;

// solver parameters
#define G 12000*-15.8f // external (gravitational) forces
#define REST_DENS 100.f // rest density
#define GAS_CONST 2000.f // const for equation of state
#define HP 100.f // kernel radius
#define MASS 50.f // assume all particles have the same mass
#define VISC 50.f // viscosity constant
#define DT 0.0018f // integration timestep
#define BOUND_DAMPING -1.f
#define T_PI 3.14159265358979323846
#define POLY 315.f/(65.f * T_PI * powf(HP, 9.f))
#define SP -45.f / (T_PI * powf(HP, 6.f))
#define VL 45.f / (T_PI * powf(HP, 6.f))

__kernel void pcl_edit(__global t_pcl *particles,  __global const size_t *pn, \
	__global t_pcl *pcls, __global float *energy, __global t_grp *groups)
{
	int		current = get_global_id(0);
	float	EPS = HP;
	float	djix;
	float	djiy;
	float	djiz;
	float	resolver = 0.f;
	float	HSQ = HP*HP*25;// // radius^2 for optimization
	float	EN = DT - *energy;
	int		grps[26];
	int		g_parse = 0;
	int		gid = particles[current].group;
	size_t	g_num = groups[0].groups;
	t_vect	fpress = {0.f, 0.f, 0.f};
	t_vect	fgrav = {0.f, 0.f, 0.f};
	t_vect	fvisc = {0.f, 0.f, 0.f};
	// printf("Current particle belogs to group of coordinates %i %i %i\n", groups[gid].coord[0], groups[gid].coord[1], groups[gid].coord[2]);
	// for (size_t i = 0; i < g_num; i++) {
	// 	if (MX && MY && MZ)
	// 		grps[g_parse++] = i;
	// 	else if (MX && MY && EZ)
	// 		grps[g_parse++] = i;
	// 	else if (MX && MY && LZ)
	// 		grps[g_parse++] = i;
	// 	else if (MX && EY && MZ)
	// 		grps[g_parse++] = i;
	// 	else if (MX && EY && EZ)
	// 		grps[g_parse++] = i;
	// 	else if (MX && EY && LZ)
	// 		grps[g_parse++] = i;
	// 	else if (MX && LY && MZ)
	// 		grps[g_parse++] = i;
	// 	else if (MX && LY && EZ)
	// 		grps[g_parse++] = i;
	// 	else if (MX && LY && LZ)
	// 		grps[g_parse++] = i;
	// 	else if (EX && MY && MZ)
	// 		grps[g_parse++] = i;
	// 	else if (EX && MY && EZ)
	// 		grps[g_parse++] = i;
	// 	else if (EX && MY && LZ)
	// 		grps[g_parse++] = i;
	// 	else if (EX && EY && MZ)
	// 		grps[g_parse++] = i;
	// 	else if (EX && EY && LZ)
	// 		grps[g_parse++] = i;
	// 	else if (EX && LY && MZ)
	// 		grps[g_parse++] = i;
	// 	else if (EX && LY && EZ)
	// 		grps[g_parse++] = i;
	// 	else if (EX && LY && LZ)
	// 		grps[g_parse++] = i;
	// 	else if (LX && MY && MZ)
	// 		grps[g_parse++] = i;
	// 	else if (LX && MY && EZ)
	// 		grps[g_parse++] = i;
	// 	else if (LX && MY && LZ)
	// 		grps[g_parse++] = i;
	// 	else if (LX && EY && MZ)
	// 		grps[g_parse++] = i;
	// 	else if (LX && EY && EZ)
	// 		grps[g_parse++] = i;
	// 	else if (LX && EY && LZ)
	// 		grps[g_parse++] = i;
	// 	else if (LX && LY && MZ)
	// 		grps[g_parse++] = i;
	// 	else if (LX && LY && EZ)
	// 		grps[g_parse++] = i;
	// 	else if (LX && LY && LZ)
	// 		grps[g_parse++] = i;
	// }
	particles[current].rho = 0.0f;
	// printf("Particle %i will be compared with pcls from %i groups:\n", current, g_parse);

	for (size_t j = 0; j < *pn; j++) {
		// printf("		Comparing pcl %i to pcl %i\n", pcl_index, current);
		djix = particles[j].posx - particles[current].posx;
		djiy = particles[j].posy - particles[current].posy;
		djiz = particles[j].posz - particles[current].posz;
		resolver = fabs(powf(djix, 2) + powf(djiy, 2) + powf(djiz, 2));
		if(resolver < HSQ)
			particles[current].rho += MASS * POLY * powf(HSQ - resolver, 2.f);
	}
	particles[current].p = GAS_CONST * (particles[current].rho - REST_DENS);
	for (size_t j = 0; j < *pn; j++) {
		if(current == (int)j)
			continue ;
		djix = particles[j].posx - particles[current].posx;
		djiy = particles[j].posy - particles[current].posy;
		djiz = particles[j].posz - particles[current].posz;
		resolver = sqrt(fabs(powf(djix, 2) + powf(djiy, 2) + powf(djiz, 2)));
		if(resolver < HP && resolver != 0.f && particles[j].rho != 0.f)
		{
			fpress.x += -djix / resolver * MASS * (particles[current].p + particles[j].p) /(2.f * particles[j].rho) * SP * powf(HP-resolver, 2.f);
			fpress.y += -djiy / resolver * MASS * (particles[current].p + particles[j].p) /(2.f * particles[j].rho) * SP * powf(HP-resolver, 2.f);
			fpress.z += -djiz / resolver * MASS * (particles[current].p + particles[j].p) /(2.f * particles[j].rho) * SP * powf(HP-resolver, 2.f);
			fvisc.x += VISC * MASS * (particles[j].vx - particles[current].vx) / particles[j].rho * VL * (HP-resolver);
			fvisc.y += VISC * MASS * (particles[j].vy - particles[current].vy) / particles[j].rho * VL * (HP-resolver);
			fvisc.z += VISC * MASS * (particles[j].vz - particles[current].vz) / particles[j].rho * VL * (HP-resolver);
		}
	}
	for (int g = 0; g < g_parse; g++) {
		int index = grps[g];
		size_t pcl_ingrp = groups[index].pcls;
		for (size_t j = 0; j < pcl_ingrp; j++) {
			int pcl_index = groups[index].pcl[j];
			// printf("		Comparing pcl %i to pcl %i\n", pcl_index, current);
			djix = particles[pcl_index].posx - particles[current].posx;
			djiy = particles[pcl_index].posy - particles[current].posy;
			djiz = particles[pcl_index].posz - particles[current].posz;
			resolver = fabs(powf(djix, 2) + powf(djiy, 2) + powf(djiz, 2));
			if(resolver < HSQ)
				particles[current].rho += MASS * POLY * powf(HSQ - resolver, 2.f);
		}
	}
	particles[current].p = GAS_CONST * (particles[current].rho - REST_DENS);
	for (int g = 0; g < g_parse; g++) {
		int index = grps[g];
		size_t pcl_ingrp = groups[index].pcls;
		for (size_t j = 0; j < pcl_ingrp; j++) {
			int pcl_index = groups[index].pcl[j];
			if(current == (int)j)
				continue ;
			djix = particles[pcl_index].posx - particles[current].posx;
			djiy = particles[pcl_index].posy - particles[current].posy;
			djiz = particles[pcl_index].posz - particles[current].posz;
			resolver = sqrt(fabs(powf(djix, 2) + powf(djiy, 2) + powf(djiz, 2)));
			if(resolver < HP && resolver != 0.f && particles[pcl_index].rho != 0.f)
			{
				fpress.x += -djix / resolver * MASS * (particles[current].p + particles[pcl_index].p) /(2.f * particles[pcl_index].rho) * SP * powf(HP-resolver, 2.f);
				fpress.y += -djiy / resolver * MASS * (particles[current].p + particles[pcl_index].p) /(2.f * particles[pcl_index].rho) * SP * powf(HP-resolver, 2.f);
				fpress.z += -djiz / resolver * MASS * (particles[current].p + particles[pcl_index].p) /(2.f * particles[pcl_index].rho) * SP * powf(HP-resolver, 2.f);
				fvisc.x += VISC * MASS * (particles[pcl_index].vx - particles[current].vx) / particles[pcl_index].rho * VL * (HP-resolver);
				fvisc.y += VISC * MASS * (particles[pcl_index].vy - particles[current].vy) / particles[pcl_index].rho * VL * (HP-resolver);
				fvisc.z += VISC * MASS * (particles[pcl_index].vz - particles[current].vz) / particles[pcl_index].rho * VL * (HP-resolver);
			}
		}
	}
	fgrav.x = 0;
	fgrav.y = 0;
	fgrav.z = G * particles[current].rho;
	particles[current].fx = fpress.x + fvisc.x + fgrav.x;
	particles[current].fy = fpress.y + fvisc.y + fgrav.y;
	particles[current].fz = fpress.z + fvisc.z + fgrav.z;
	if (particles[current].rho != 0.f) {
		particles[current].vx += EN * particles[current].fx / particles[current].rho;
		particles[current].vy += EN * particles[current].fy / particles[current].rho;
		particles[current].vz += EN * particles[current].fz / particles[current].rho;
		if (particles[current].vz > 10000)
			particles[current].vz = 10000;
		if (particles[current].vz < -10000)
			particles[current].vz = -10000;
		if (particles[current].vy > 10000)
			particles[current].vy = 10000;
		if (particles[current].vy < -10000)
			particles[current].vy = -10000;
		if (particles[current].vx > 10000)
			particles[current].vx = 10000;
		if (particles[current].vx < -10000)
			particles[current].vx = -10000;
	}
	particles[current].posx += EN * particles[current].vx;
	particles[current].posy += EN * particles[current].vy;
	particles[current].posz += EN * particles[current].vz;

	// enforce boundary conditions
	if(particles[current].posx - EPS <= 0.0f)
	{
		particles[current].vx  *= BOUND_DAMPING + 0.6;
		particles[current].posx = EPS + fabs((float)cos((float)current) * 10);
	}
	if(particles[current].posx + EPS >= 2000)
	{
		particles[current].vx *= BOUND_DAMPING+ 0.6;
		particles[current].posx = 2000 - EPS - fabs((float)cos((float)current) * 10);
	}
	if(particles[current].posy - EPS <= 0.0f)
	{
		particles[current].vy *= BOUND_DAMPING+ 0.6;
		particles[current].posy = EPS + fabs((float)cos((float)current) * 10);
	}
	if(particles[current].posy + EPS >= 2000)
	{
		particles[current].vy *= BOUND_DAMPING+ 0.6;
		particles[current].posy = 2000 - EPS - fabs((float)cos((float)current) * 10);
	}
	if(particles[current].posz - EPS <= 0.0f)
	{
		particles[current].vz *= BOUND_DAMPING+ 0.2;
		particles[current].posz = EPS + fabs((float)cos((float)current) * 10);
	}
	if(particles[current].posz + EPS >= 2000)
	{
		particles[current].vz *= BOUND_DAMPING+ 0.2;
		particles[current].posz = 2000 - EPS - fabs((float)cos((float)current) * 10);
	}

	pcls[current].posx = particles[current].posx;
	pcls[current].posy = particles[current].posy;
	pcls[current].posz = particles[current].posz;
	pcls[current].vx = particles[current].vx;
	pcls[current].vy = particles[current].vy;
	pcls[current].vz = particles[current].vz;
	pcls[current].fx = particles[current].fx;
	pcls[current].fy = particles[current].fy;
	pcls[current].fz = particles[current].fz;
	pcls[current].p = particles[current].p;
	pcls[current].rho = particles[current].rho;
}
