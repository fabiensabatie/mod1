/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <fsabatie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 17:23:49 by fsabatie          #+#    #+#             */
/*   Updated: 2018/04/06 21:18:04 by vlay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOD1_H
# define MOD1_H
# include "libft.h"
# include <math.h>
# include <GLFW/glfw3.h>
# include <OpenCL/opencl.h>

# define LOG_BUFFSIZE 10000000

typedef struct		s_shader
{
	GLuint			id;
	GLchar			*src;
	GLuint			prog;
	GLenum			type;
	struct			s_info
	{
		GLint		success;
		GLint		msg_len; // Taille du message retourné par OpenGL après la compilation.
		GLchar		*log; // Message retourné.
		GLenum		type;
	}				info;
}					t_shader;

typedef struct		s_kernel
{
	cl_program			prog;
	cl_int				ret;
	cl_context			context;
	cl_command_queue 	command_queue;
	cl_kernel			kernel;
	cl_device_id		device_id;
	cl_mem				particles_mem_obj;
	cl_mem				p_mem_obj;
	cl_mem				pcls_mem_obj;
	cl_mem				energy_obj;
}					t_kernel;

typedef struct	s_vec3D
{
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

typedef struct	s_grp
{
	int			coord[3];
	int			pcl[1000];
	size_t		groups;
	size_t		pcls;
	size_t		ngrps[26];
	short		n_groups_number;
}				t_grp;

typedef struct	s_render
{
	GLFWwindow	*win;
	float		**points;
	t_pcl		*particles;
	t_grp		*groups;
	t_kernel	*k;
	size_t		part_number;
	int			size;
	float		zmax;
	GLuint		draw_mod;
	float		rotY;
	float 		rotX;
	int 		psy;
	int			m_height;
	size_t		grp_n;
}				t_render;


int				init_win(t_render *r);
int				render(t_render *r);
int				processKernel(t_render *r);
void			event(GLFWwindow* window, int key, int scancode, int action, int mods);
void			updateParticlesState(t_render *r);
float			interpolation(short n, float **points, float X, float Y);
float			**parser(t_render *r, char *path);
t_pcl			*initParticles(t_render *r);
GLchar			*getFileSource(char *filename);
t_kernel		*build_kernel(char *path);
t_shader		*build_shader(char *filename, GLenum type, GLuint prog_id, t_bool prog);


#endif
