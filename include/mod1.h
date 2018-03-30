/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wwatkins <wwatkins@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/29 17:23:49 by wwatkins          #+#    #+#             */
/*   Updated: 2016/12/12 11:34:21 by wwatkins         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H
# include "libft.h"
# include <math.h>
# include <GLFW/glfw3.h>
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

typedef struct	s_render
{
	GLFWwindow*	win;
}				t_render;

int				init_win(t_render *r);
int				render(t_render *r);
t_shader		*build_shader(char *filename, GLenum type, GLuint prog_id, \
				t_bool prog);
float 			getZ(short n, float points[n][3],  float X, float Y);
#endif
