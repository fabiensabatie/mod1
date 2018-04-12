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


t_kernel *getErr(t_kernel *k)
{
	size_t log_size;
	clGetProgramBuildInfo(k->prog, k->device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
	char *log = (char*)malloc(log_size);
	clGetProgramBuildInfo(k->prog, k->device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
	printf("%s\n", log);
	return (NULL);
}

t_kernel *build_kernel(char *file) {
	t_kernel *k;
	char *src;
	size_t len;

	if (!(src = (char*)getFileSource(file))
	|| !(k = (t_kernel*)malloc(sizeof(t_kernel))))
		return (NULL);
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	k->device_id = NULL;
	clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &(k->device_id), &ret_num_devices);
	k->context = clCreateContext(NULL, 1, &(k->device_id), NULL, NULL, &(k->ret));
	k->command_queue = clCreateCommandQueue(k->context, k->device_id, 0, &(k->ret));
	len = (size_t)ft_strlen(src);
	k->prog = clCreateProgramWithSource(k->context, 1, (const char **)&src, (const size_t *)&len, &(k->ret));
	free (src);
	clBuildProgram(k->prog, 1, &(k->device_id), NULL, NULL, NULL);
	if (!(k->kernel = clCreateKernel(k->prog, "pcl_edit", &(k->ret))))
		return getErr(k);
	return (k);
}


t_list *ft_searchlist(t_list *lst, size_t hash)
{
	if (!lst)
		return (NULL);
	while (lst) {
		if (lst->content_size == hash){
			return ((t_list*)lst->content);
		}
		lst = lst->next;
	}
	return (NULL);
}

t_pcl *sort_pcl(t_render *r, t_pcl *pcls)
{
	float HP = 100;
	float	interval = (2000-HP)/HP;
	t_list *list = NULL;
	t_list *group;
	size_t pc = 0;
	size_t hash;
	size_t g = 0;
	t_pcl *pcl;

	if (!(pcl = (t_pcl*)malloc(sizeof(t_pcl) * r->part_number)))
		return (NULL);
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
		if (!(group = ft_searchlist(list, hash))) { // The group doesn not exist in the list
			group = ft_lstlink(ft_lstlink(&pcls[i], hash), hash);
			if (!list){
				list = group;
			}
			else
				ft_lstpushback(list, group);
		}
		else {
			t_list *new = ft_lstlink(&pcls[i], hash);
			ft_lstpushback(group, new);
			pc++;
		}
	}
	g = 0;
	pc = 0;
	group = list;
	t_list *p_list;
	t_list *p_group;
	t_list *prev_list;
	while (group)
	{
		p_group = group;
		p_list = (t_list*)group->content;
		while (p_list) {
			prev_list = p_list;
			t_pcl *p = p_list->content;
			pcl[pc] = *p;
			pcl[pc++].group = (int)g;
			p_list = p_list->next;
			free(prev_list);
		}
		group = group->next;
		free(p_group);
		g++;
	}
	// printf("%li groups\n", g);
	return (pcl);
}


int processKernel(t_render *r)
{
	t_kernel *k = r->k;
	t_pcl *particles = r->particles;
	t_pcl *pcls;
	size_t *p = &(r->part_number);

	// particles = sort_pcl(r, particles);
	if (!(pcls = (t_pcl*)malloc(sizeof(t_pcl) * r->part_number)))
		return (0);
	k->particles_mem_obj = clCreateBuffer(k->context, CL_MEM_WRITE_ONLY, r->part_number * sizeof(t_pcl), NULL, &(k->ret));
	k->p_mem_obj = clCreateBuffer(k->context, CL_MEM_READ_ONLY, sizeof(size_t), NULL, &(k->ret));
	k->pcls_mem_obj = clCreateBuffer(k->context, CL_MEM_WRITE_ONLY, r->part_number * sizeof(t_pcl), NULL, &(k->ret));
	clEnqueueWriteBuffer(k->command_queue, k->particles_mem_obj, CL_FALSE, 0, r->part_number * sizeof(t_pcl), particles, 0, NULL, NULL);
	clEnqueueWriteBuffer(k->command_queue, k->p_mem_obj, CL_FALSE, 0, sizeof(size_t), p, 0, NULL, NULL);
	clSetKernelArg(k->kernel, 0, sizeof(cl_mem), (void *)&(k->particles_mem_obj));
	clSetKernelArg(k->kernel, 1, sizeof(cl_mem), (void *)&(k->p_mem_obj));
	clSetKernelArg(k->kernel, 2, sizeof(cl_mem), (void *)&(k->pcls_mem_obj));
	size_t global_item_size = 32 - (r->part_number % 32) + (r->part_number); // Process the entire lists
	size_t local_item_size = 32; // Divide work items into groups of 64
	clEnqueueNDRangeKernel(k->command_queue, k->kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	clEnqueueReadBuffer(k->command_queue, k->pcls_mem_obj, CL_TRUE, 0, r->part_number * sizeof(t_pcl), pcls, 0, NULL, NULL);
	// free(particles);
	// free(r->particles);
	float f = 0;
	for (size_t i = 0; i < r->part_number; i++) {
		f += fabs(pcls[i].vx) + fabs(pcls[i].vy) + fabs(pcls[i].vz);
	}
	printf("Forces: %f\n", f);
	r->particles = pcls;
	return 0;
}
