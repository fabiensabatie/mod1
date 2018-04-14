/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:37:32 by f
sabatie          #+#    #+#             */
/*   Updated: 2018/03/26 14:37:33 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod1.h"
#define W_IBLOCK 512
#define WSIZE W_IBLOCK - (r->part_number % W_IBLOCK) + r->part_number
#define HPI 5
#define INTERVAL (2000-HPI)/HPI
#define EX (grps[i].coord[0] == grps[j].coord[0])
#define LX (grps[i].coord[0] == grps[j].coord[0] - 1)
#define MX (grps[i].coord[0] == grps[j].coord[0] + 1)
#define EY (grps[i].coord[1] == grps[j].coord[1])
#define LY (grps[i].coord[1] == grps[j].coord[1] - 1)
#define MY (grps[i].coord[1] == grps[j].coord[1] + 1)
#define EZ (grps[i].coord[2] == grps[j].coord[2])
#define LZ (grps[i].coord[2] == grps[j].coord[2] - 1)
#define MZ (grps[i].coord[2] == grps[j].coord[2] + 1)
#define IX (grps[i].coord[0] > 0)
#define IY (grps[i].coord[1] > 0)
#define IZ (grps[i].coord[2] > 0)
#define JX (grps[j].coord[0] > 0)
#define JY (grps[j].coord[1] > 0)
#define JZ (grps[j].coord[2] > 0)
#define MIX (grps[i].coord[0] < (int)2000/INTERVAL)
#define MIY (grps[i].coord[1] < (int)2000/INTERVAL)
#define MIZ (grps[i].coord[2] < (int)2000/INTERVAL)
#define MJX (grps[j].coord[0] < (int)2000/INTERVAL)
#define MJY (grps[j].coord[1] < (int)2000/INTERVAL)
#define MJZ (grps[j].coord[2] < (int)2000/INTERVAL)

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


t_list *ft_searchlist(t_list *lst, int coord[3])
{
	size_t i = 0;
	if (!lst)
		return (NULL);
	while (lst) {
		if (i % 2 == 0) {
			int *c = (int*)lst->content;
			if (c[0] == coord[0] && c[1] == coord[1] && c[2] == coord[2]){
				return ((t_list*)lst);
			}
		}
		lst = lst->next;
		i++;
	}
	return (NULL);
}

t_grp *getGroupsNeighbors(t_render *r, t_grp *grps)
{
	for (size_t i = 0; i < r->grp_n; i++) {
		grps[i].n_groups_number = 0;
		if (grps[i].n_groups_number < 26)
			for (size_t j = 0; j < r->grp_n; j++) {
				if (i == j)
					continue;
				if (MX && MY && MZ && IX && IY && IZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (MX && MY && EZ && IX && IY)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (MX && MY && LZ && IX && IY && IZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (MX && EY && MZ && IX && IZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (MX && EY && EZ && IX)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (MX && EY && LZ && IX && MIZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (MX && LY && MZ && IX && MIY && IZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (MX && LY && EZ && IX && MIY)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (MX && LY && LZ && IX && MIY && MIZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (EX && MY && MZ && IY && IZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (EX && MY && EZ && IY)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (EX && MY && LZ && IY && MIZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (EX && EY && MZ && IZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (EX && EY && LZ && MIZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (EX && LY && MZ && MIY && IZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (EX && LY && EZ && MIY)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (EX && LY && LZ && MIY && MIZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (LX && MY && MZ && MIX && IY && IZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (LX && MY && EZ && MIX && IY)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (LX && MY && LZ && MIX && IY && MIZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (LX && EY && MZ && MIX && IZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (LX && EY && EZ && MIX)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (LX && EY && LZ && MIX && MIZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (LX && LY && MZ && MIX && MIY && IZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (LX && LY && EZ && MIX && MIY)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
				else if (LX && LY && LZ && MIX && MIY && MIZ)
					grps[i].ngrps[grps[i].n_groups_number++] = j;
			}
	}
	return (grps);
}

t_grp *assignGroups(t_render *r, t_pcl *pcls, t_list *list)
{
	t_grp	*grps;
	t_list	*group = list;
	t_list	*prevg;
	t_list	*prevp;
	size_t	g = 0;
	if (!(grps = (t_grp*)malloc(sizeof(t_grp) * r->grp_n)))
		return (NULL);
	while (group) {
		prevg = group;
		grps[g].coord[0] = ((int*)group->content)[0];
		grps[g].coord[1] = ((int*)group->content)[1];
		grps[g].coord[2] = ((int*)group->content)[2];
		grps[g].groups = r->grp_n;
		grps[g].pcls = group->content_size;
		group = group->next;
		free(prevg);
		prevg = group;
		t_list *p_list = (t_list*)group->content;
		for (size_t i = 0; i < grps[g].pcls; i++) {
			prevp = p_list;
			grps[g].pcl[i] = (int)p_list->content_size;
			pcls[grps[g].pcl[i]].group = g;
			p_list = p_list->next;
			free(prevp);
		}
		g++;
		group = group->next;
		free(prevg);
	}
	return (grps);
}

t_grp *getGroups(t_render *r, t_pcl *pcls)
{
	t_list	*list = NULL;
	t_list	*group;
	size_t	pc = 0;
	size_t	g = 0;
	int		coord[3];

	for (size_t i = 0; i < r->part_number; i++)
	{
		coord[0] = 0;
		coord[1] = 0;
		coord[2] = 0;
		for (float j = 0; INTERVAL * j + HPI < 2000; j++)
		{
			if (INTERVAL * j + HPI > pcls[i].posx)
				coord[0] = (coord[0] == 0) ? j - 1 : coord[0];
			if (INTERVAL * j + HPI > pcls[i].posy)
				coord[1] = (coord[1] == 0) ? j - 1 : coord[1];
			if (INTERVAL * j + HPI > pcls[i].posz)
				coord[2] = (coord[2] == 0) ? j - 1 : coord[2];
		}
		if (!(group = ft_searchlist(list, coord))) { // The group doesn not exist in the list
			t_list *coordinates = ft_lstlink(ft_memnewcpy(coord, sizeof(int), 3), 1);
			group = ft_lstlink(ft_lstlink(&pcls[i], i), 0);
			g++;
			ft_lstpushback(coordinates, group);
			if (!list)
				list = coordinates;
			else
				ft_lstadd(&list, coordinates);
		}
		else {
			(group->content_size)++;
			group = group->next;
			t_list *new = ft_lstlink(&pcls[i], i);
			ft_lstpushback(group->content, new);
			pc++;
		}
	}
	r->grp_n = g;
	return (getGroupsNeighbors(r,assignGroups(r, pcls, list)));
}


int processKernel(t_render *r)
{
	t_kernel *k = r->k;
	t_pcl *particles = r->particles;
	t_pcl *pcls;
	size_t *p = &(r->part_number);
	static float energy = 0;

	r->groups = getGroups(r, particles);

	// for (size_t i = 0; i < r->grp_n; i++) {
	// 	printf("%i %i %i\n", r->groups[i].coord[0], r->groups[i].coord[1], r->groups[i].coord[2]);
	// }
	if (!(pcls = (t_pcl*)malloc(sizeof(t_pcl) * r->part_number)))
		return (0);
	cl_mem particles_mem_obj = clCreateBuffer(k->context, CL_MEM_WRITE_ONLY, r->part_number * sizeof(t_pcl), NULL, &(k->ret));
	cl_mem p_mem_obj = clCreateBuffer(k->context, CL_MEM_READ_ONLY, sizeof(size_t), NULL, &(k->ret));
	cl_mem pcls_mem_obj = clCreateBuffer(k->context, CL_MEM_WRITE_ONLY, r->part_number * sizeof(t_pcl), NULL, &(k->ret));
	cl_mem energy_obj = clCreateBuffer(k->context, CL_MEM_READ_ONLY, sizeof(float), NULL, &(k->ret));
	cl_mem group_obj = clCreateBuffer(k->context, CL_MEM_READ_ONLY, sizeof(t_grp) * r->grp_n, NULL, &(k->ret));
	clEnqueueWriteBuffer(k->command_queue, particles_mem_obj, CL_FALSE, 0, r->part_number * sizeof(t_pcl), particles, 0, NULL, NULL);
	clEnqueueWriteBuffer(k->command_queue, p_mem_obj, CL_FALSE, 0, sizeof(size_t), p, 0, NULL, NULL);
	clEnqueueWriteBuffer(k->command_queue, energy_obj, CL_FALSE, 0, sizeof(float), &energy, 0, NULL, NULL);
	clEnqueueWriteBuffer(k->command_queue, group_obj, CL_FALSE, 0, sizeof(t_grp) * r->grp_n, r->groups, 0, NULL, NULL);
	clSetKernelArg(k->kernel, 0, sizeof(cl_mem), (void *)&(particles_mem_obj));
	clSetKernelArg(k->kernel, 1, sizeof(cl_mem), (void *)&(p_mem_obj));
	clSetKernelArg(k->kernel, 2, sizeof(cl_mem), (void *)&(pcls_mem_obj));
	clSetKernelArg(k->kernel, 3, sizeof(cl_mem), (void *)&(energy_obj));
	clSetKernelArg(k->kernel, 4, sizeof(cl_mem), (void *)&(group_obj));
	size_t global_item_size = 128 - (r->part_number % 128) + (r->part_number); // Process the entire lists
	size_t local_item_size = 128; // Divide work items into groups of 64
	clEnqueueNDRangeKernel(k->command_queue, k->kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	clEnqueueReadBuffer(k->command_queue, pcls_mem_obj, CL_TRUE, 0, r->part_number * sizeof(t_pcl), pcls, 0, NULL, NULL);
	// free(particles);
	// free(r->particles);
	// for (size_t i = 0; i < r->part_number; i++) {
	// 	printf("%f %f %f\n", pcls[i].posx, pcls[i].posy, pcls[i].posz);
	// }
	// printf("Forces: %f\n", f);
	r->particles = pcls;
	energy += 0.0000005;
	if (energy > 0.0018)
		energy = 0.0018;
	return 0;
}
