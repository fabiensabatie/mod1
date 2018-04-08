/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mod1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wwatkins <wwatkins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/29 17:23:49 by wwatkins          #+#    #+#             */
/*   Updated: 2018/04/06 21:18:04 by vlay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H
# include "libft.h"
# include <math.h>
# include <GLFW/glfw3.h>
# include <OpenCL/opencl.h>

# define LOG_BUFFSIZE 10000000
# define IX(x, y, z) ((x) + (y) * N + (z) * N * N)

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

typedef struct	s_vec3D {

	float		x;
	float		y;
	float		z;
}				vec3D;

typedef struct	s_pcl {

	vec3D		pos; // Position
	vec3D		v; // Velocity
	vec3D		f; // Forces (gravity)
	float		rho; // Density
	float		p; // Pressure
}				t_pcl;

typedef struct	s_render
{
	GLFWwindow	*win;
	float		**points;
	t_pcl		**particles;
	size_t		part_number;
	int			size;
	float		zmax;
	GLuint		draw_mod;
	float		rotY;
	float 		rotX;
	int 		psy;
	int			m_height;
}				t_render;

typedef struct s_cube
{
	int size;
	float dt;
	float diff;
	float visc;

	float *s;
	float *density;

	float *Vx;
	float *Vy;
	float *Vz;

	float *Vx0;
	float *Vy0;
	float *Vz0;
}				t_cube;

int				init_win(t_render *r);
int				render(t_render *r);
t_shader		*build_shader(char *filename, GLenum type, GLuint prog_id, \
				t_bool prog);
float			interpolation(short n, float **points, float X, float Y);
float			**parser(t_render *r, char *path);
void			event(GLFWwindow* window, int key, int scancode, int action, \
				int mods);
t_pcl			**initParticles(t_render *r);
void			updateParticlesState(t_render *r);
t_cube			*t_cubeCreate(int size, int diffusion, int viscosity, float dt);
void			t_cubeFree(t_cube *cube);
void			t_cubeAddDensity(t_cube *cube, int x, int y, int z, float amount);
void			t_cubeAddVelocity(t_cube *cube, int x, int y, int z, float amountX, float amountY, float amountZ);
void			t_cubeStep(t_cube *cube);

#endif
