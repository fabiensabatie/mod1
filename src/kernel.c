/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:37:32 by fsabatie          #+#    #+#             */
/*   Updated: 2018/03/26 14:37:33 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod1.h"
#define W_IBLOCK 512
#define WSIZE W_IBLOCK - (r->part_number % W_IBLOCK) + r->part_number

#define HP 100.f // kernel radius

t_kernel *getErr(t_kernel *k)
{
	size_t log_size;
	clGetProgramBuildInfo(k->prog, k->device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
	char *log = (char*)malloc(log_size);
	clGetProgramBuildInfo(k->prog, k->device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
	printf("%s\n", log);
	return (NULL);
}

t_kernel *build_kernel(t_render *r, char *file) {
	t_kernel *k;
	char *src;
	size_t len;

	if (!(src = (char*)getFileSource(file))
	|| !(k = (t_kernel*)malloc(sizeof(t_kernel))))
		return (NULL);
	(void)r;
	// Get platform and device information
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	k->device_id = NULL;
	clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &(k->device_id), &ret_num_devices);
	// Create an OpenCL context
	k->context = clCreateContext(NULL, 1, &(k->device_id), NULL, NULL, &(k->ret));
	// Create a command queue
	k->command_queue = clCreateCommandQueue(k->context, k->device_id, 0, &(k->ret));
	// Create a program from the kernel source
	len = (size_t)ft_strlen(src);
	k->prog = clCreateProgramWithSource(k->context, 1, (const char **)&src, (const size_t *)&len, &(k->ret));
	// Build the program
	clBuildProgram(k->prog, 1, &(k->device_id), NULL, NULL, NULL);
	// Create the OpenCL kernel
	if (!(k->kernel = clCreateKernel(k->prog, "pcl_edit", &(k->ret))))
		return getErr(k);
	return (k);
}


#include <time.h>



t_list *ft_searchlist(t_list *lst, size_t hash)
{
	if (!lst)
		return NULL;
	while (lst) {
		// printf("hash of list = %li at %p and hash = %li\n", lst->content_size, lst, hash);
		if (lst->content_size == hash)
			return ((t_list*)lst);
		lst = lst->next;
	}
	return (NULL);
}

t_pcl *sort_pcl(t_render *r, t_pcl *pcls)
{
	float	interval = (CUBEX-HP)/160;
	t_list *list = NULL;
	t_list *group;
	size_t pc = 0;
	size_t hash;
	size_t g = 0;
	t_pcl *pcl = malloc(sizeof(t_pcl) * r->part_number);
	for (size_t i = 0; i < r->part_number; i++)
	{
		size_t x = 0, y = 0, z = 0;
		for (float j = 0; interval * j + HP < 1000; j++)
		{
			if (interval * j + HP > pcls[i].posx)
				x = (x == 0) ? j - 1 : x;
			if (interval * j + HP > pcls[i].posy)
				y = (y == 0) ? j - 1 : y;
			if (interval * j + HP > pcls[i].posz)
				z = (z == 0) ? j - 1 : z;
		}
		hash = x + y * 1000 + z * 10000;
		if (!(group = ft_searchlist(list, hash))) // Groupe non trouvé
		{
			group = ft_lstlink(&pcls[i], hash, sizeof(t_pcl*)); // Créer un groupe.
			// printf("Created the group: %p - hash: %li*%li*%li = %li\n", group, x, y, z, hash);
			g++;
			pc++;
			if (!list) // Si la liste n'existe pas.
				list = ft_lstlink(group, hash, sizeof(t_list*)); // Créer la liste.
			else {
				ft_lstadd(&list, ft_lstlink(group, hash, sizeof(t_list*))); // Ajoute le groupe a la liste.
				// printf("Now list is at: %p\n", list);
			}

		}
		else { // Si le groupe a éé trouvé.
			t_list **s = &list;
			pc++;
			ft_lstadd(&group, ft_lstlink(&pcls[i], hash, sizeof(t_pcl*))); // Ajoute la valeur au groupe.
			while (list) {
				// printf("Parsing list %p\n", list);
				if (list->content_size == hash) {
					// printf("Replacing %p by %p\n", list, group);
					list->content = group;
					break;
				}
				list = list->next;
			}
			list = *s;
		}
	}
	printf("Created %li groups.\n", g);
	printf("Copied %li pcls.\n", pc);
	t_list *s;
	t_list *c;
	s = list;
	g = 0;
	pc = 0;
	while (s)
	{
		c = (t_list*)s->content;
		while (c)
		{
			t_pcl *p = (t_pcl*)c->content;
			if ((int)p->posx != 0 && p->posy != 0.0f && p->posz != 0.0f) {
				printf("Particle %li: %.10f %.10f %.10f in G%li\n", pc, p->posx, p->posy, p->posz, g);
				if (pc < r->part_number) {
					pcl[pc] = *p;
					pcl[pc++].group = g;
				}

			}
			c = c->next;
		}
		s = s->next;
		g++;
	}
	free(pcls);
	return (pcls);
}


int processKernel(t_render *r)
{
	t_kernel *k = r->k;
	t_pcl *particles = r->particles;
	static t_pcl *pcls = NULL;
	size_t *p = &(r->part_number);

	particles = sort_pcl(r, particles);
	if (!pcls && !(pcls = (t_pcl*)malloc(sizeof(t_pcl) * r->part_number)))
		return (0);
	k->particles_mem_obj = clCreateBuffer(k->context, CL_MEM_WRITE_ONLY, r->part_number * sizeof(t_pcl), NULL, &(k->ret));
	k->p_mem_obj = clCreateBuffer(k->context, CL_MEM_READ_ONLY, sizeof(size_t), NULL, &(k->ret));
	k->pcls_mem_obj = clCreateBuffer(k->context, CL_MEM_WRITE_ONLY, r->part_number * sizeof(t_pcl), NULL, &(k->ret));
	clEnqueueWriteBuffer(k->command_queue, k->particles_mem_obj, CL_FALSE, 0, r->part_number * sizeof(t_pcl), particles, 0, NULL, NULL);
	clEnqueueWriteBuffer(k->command_queue, k->p_mem_obj, CL_FALSE, 0, sizeof(size_t), p, 0, NULL, NULL);
	clSetKernelArg(k->kernel, 0, sizeof(cl_mem), (void *)&(k->particles_mem_obj));
	clSetKernelArg(k->kernel, 1, sizeof(cl_mem), (void *)&(k->p_mem_obj));
	clSetKernelArg(k->kernel, 2, sizeof(cl_mem), (void *)&(k->pcls_mem_obj));
	size_t global_item_size[3] = {WSIZE, WSIZE, WSIZE}; //32 - ((r->part_number + 1) % 32) + (r->part_number + 1); // Process the entire lists
	size_t local_item_size[3] = {W_IBLOCK, W_IBLOCK, W_IBLOCK}; // Divide work items into groups of 64
	clEnqueueNDRangeKernel(k->command_queue, k->kernel, 1, NULL, global_item_size, local_item_size, 0, NULL, NULL);
	clEnqueueReadBuffer(k->command_queue, k->pcls_mem_obj, CL_TRUE, 0, r->part_number * sizeof(t_pcl), pcls, 0, NULL, NULL);
	r->particles = pcls;
	return 0;
}
