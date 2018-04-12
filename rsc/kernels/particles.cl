typedef struct	s_vec3D {

	float		x;
	float		y;
	float		z;
}				vec3D;

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

// solver parameters
#define G1 0.f // external (gravitational) forces
#define G2 12000*-9.8f // external (gravitational) forces
#define REST_DENS 100.f // rest density
#define GAS_CONST 2000.f // const for equation of state
#define HP 100.f // kernel radius
#define MASS 20.f // assume all particles have the same mass
#define VISC 400.f // viscosity constant
#define DT 0.001f // integration timestep
#define BOUND_DAMPING -0.05f
#define T_PI 3.14159265358979323846

__kernel void pcl_edit(__global t_pcl *particles,  __global const size_t *pn \
	, __global t_pcl *pcls) {
	int i = get_global_id(0);
	float EPS = HP;
	float djix;
	float djiy;
	float djiz;
	float resolver = 0.f;
	float poly;
	float HSQ = HP*HP*60; // radius^2 for optimization
	float sp;
	float vl;
	vec3D fgrav;


	size_t s = (size_t)i;
	size_t e = (size_t)i;
	// Get the index of the current element to be processed

	poly = 315.f/(65.f * T_PI * powf(HP, 9.f));
	sp = -45.f / (T_PI * powf(HP, 6.f));
	vl = 45.f / (T_PI * powf(HP, 6.f));
	particles[i].rho = 0.0f;
	while (s > 0 && particles[s].group == particles[i].group)
		s--;
	while (e < *pn && particles[e].group == particles[i].group)
		e++;
	// printf("Computing %li pcls\n", e-s);
	for (size_t j = s; j < e; j++)
	{
		djix = particles[j].posx - particles[i].posx;
		djiy = particles[j].posy - particles[i].posy;
		djiz = particles[j].posz - particles[i].posz;
		resolver = powf(djix, 2) + powf(djiy, 2) + powf(djiz, 2); //+ pow(djiz, 2);
		if(resolver < HSQ)
			particles[i].rho += MASS * poly * powf(HSQ - resolver, 2.f);
	}
	particles[i].p = GAS_CONST * (particles[i].rho - REST_DENS);
	vec3D fpress = {0.f, 0.f, 0.f};
	vec3D fvisc = {0.f, 0.f, 0.f};
	for (size_t j = s; j < e; j++)
	{
		if(i == (int)j)
			continue ;
		djix = particles[j].posx - particles[i].posx;
		djiy = particles[j].posy - particles[i].posy;
		djiz = particles[j].posz - particles[i].posz;
		resolver = sqrt(powf(djix, 2) + powf(djiy, 2) + powf(djiz, 2));
		if(resolver < HP && resolver != 0 && particles[j].rho != 0.f)
		{
			fpress.x += -djix / resolver * MASS * (particles[i].p + particles[j].p) /(2.f * particles[j].rho) * sp * powf(HP - resolver,2.f);
			fpress.y += -djiy / resolver * MASS * (particles[i].p + particles[j].p) /(2.f * particles[j].rho) * sp * powf(HP - resolver,2.f);
			fpress.z += -djiz / resolver * MASS * (particles[i].p + particles[j].p) /(2.f * particles[j].rho) * sp * powf(HP - resolver,2.f);
			fvisc.x += VISC * MASS * (particles[j].vx - particles[i].vx) / particles[j].rho * vl * (HP - resolver);
			fvisc.y += VISC * MASS * (particles[j].vy - particles[i].vy) / particles[j].rho * vl * (HP - resolver);
			fvisc.z += VISC * MASS * (particles[j].vz - particles[i].vz) / particles[j].rho * vl * (HP - resolver);
		}
	}
	fgrav.x = 0;
	fgrav.y = 0;
	fgrav.z = G2 * particles[i].rho;
	particles[i].fx = fpress.x + fvisc.x + fgrav.x;
	particles[i].fy = fpress.y + fvisc.y + fgrav.y;
	particles[i].fz = fpress.z + fvisc.z + fgrav.z;
	if (particles[i].rho != 0.f) {
		particles[i].vx += DT * particles[i].fx / particles[i].rho;
		particles[i].vy += DT * particles[i].fy / particles[i].rho;
		particles[i].vz += DT * particles[i].fz / particles[i].rho;
	}
	particles[i].posx += DT * particles[i].vx;
	particles[i].posy += DT * particles[i].vy;
	particles[i].posz += DT * particles[i].vz;

	// enforce boundary conditions
	if(particles[i].posx - EPS < 0.0f)
	{
		particles[i].vx  *= BOUND_DAMPING;
		particles[i].posx = EPS + (float)cos((float)i);
	}
	if(particles[i].posx + EPS > 2000)
	{
		particles[i].vx *= BOUND_DAMPING;
		particles[i].posx = 2000 - EPS - (float)cos((float)i);
	}
	if(particles[i].posy - EPS < 0.0f)
	{
		particles[i].vy *= BOUND_DAMPING;
		particles[i].posy = EPS + (float)cos((float)i);
	}
	if(particles[i].posy + EPS > 2000)
	{
		particles[i].vy *= BOUND_DAMPING;
		particles[i].posy = 2000 - EPS - (float)cos((float)i);
	}
	if(particles[i].posz - EPS < 0.0f)
	{
		particles[i].vz *= BOUND_DAMPING;
		particles[i].posz = EPS + (float)cos((float)i);
	}
	if(particles[i].posz + EPS > 2000)
	{
		particles[i].vz *= BOUND_DAMPING;
		particles[i].posz = 2000 - EPS - (float)cos((float)i);
	}

	pcls[i].posx = particles[i].posx;
	pcls[i].posy = particles[i].posy;
	pcls[i].posz = particles[i].posz;
	pcls[i].vx = particles[i].vx;
	pcls[i].vy = particles[i].vy;
	pcls[i].vz = particles[i].vz;
	pcls[i].fx = particles[i].fx;
	pcls[i].fy = particles[i].fy;
	pcls[i].fz = particles[i].fz;
	pcls[i].p = particles[i].p;
	pcls[i].rho = particles[i].rho;
}