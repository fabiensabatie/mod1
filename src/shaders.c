/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsabatie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 14:37:32 by fsabatie          #+#    #+#             */
/*   Updated: 2018/03/26 14:37:33 by fsabatie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mod1.h"

GLchar		*getShaderSource(char *filename)
{
	char	*line;
	char	*src;
	int		fd;

	if (!(fd = open(filename, O_RDONLY))
		|| !(src = (char*)malloc(1)))
		return (NULL);
	while (get_next_line(fd, &line) > 0)
	{
		line = ft_strjoinfree(line, "\n");
		src = ft_strjoinfree(src, line);
		free(line);
	}
	return ((GLchar*)src);
}

int			errShaderCompilation(t_shader *s, char *filename)
{
	if (s->info.success != GL_TRUE)
	{
		// Récupération de la taille du message d'erreur
		glGetShaderiv(s->id, GL_INFO_LOG_LENGTH, &(s->info.msg_len));
		// Récupération de l'erreur
		glGetShaderInfoLog(s->id, LOG_BUFFSIZE, &(s->info.msg_len), s->info.log);
		s->info.log[s->info.msg_len] = '\0';
		// Affichage de l'erreur
		ft_printf("Erreur de compilation pour \"%s\"\n%s", filename, s->info.log);
		return (1);
	}
	return (0);
}


t_shader	*build_shader(char *filename, GLenum type, GLuint prog_id, \
			t_bool prog)
{
	t_shader *s;

	if (!(s = (t_shader*)malloc(sizeof(t_shader)))
		|| !(s->info.log = (GLchar*)malloc(sizeof(LOG_BUFFSIZE))))
		return (0);
	// Recuperation des id pour les vertex / fragment shaders
	if (!(s->id = glCreateShader(type))
		|| !(s->src = getShaderSource(filename)))
		return (0);
	// Assignation de sa source au shader
	glShaderSource(s->id, 1, (const char *const *)&(s->src), 0);
	// Compilation de la source du shader
	glCompileShader(s->id);
	// Récupération des informations sur la compilation du shader
	glGetShaderiv(s->id, GL_COMPILE_STATUS, &(s->info.success));
	// Vérifie si la compilation à échoué ou non
	if (errShaderCompilation(s, filename))
		return (NULL);
	// Créé le programme
	s->prog = (prog == TRUE) ? prog_id : glCreateProgram();
	// Attache le shader au programme
	glAttachShader(s->prog, s->id);
	glLinkProgram(s->prog);
	return (s);
}
