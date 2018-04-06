#include "mod1.h"

int		checkw(char *word)
{
	if (ft_strcount(word, ',') != 2)
		return (0);
	if (ft_strcount(word, '(') != 1)
		return (0);
	if (ft_strcount(word, ')') != 1)
		return (0);
	if (*word != '(')
		return 0;
	if (word[ft_strlen(word) - 1] != ')')
		return 0;
	return 1;
}

float	**parser(t_render *r, char *path)
{
	char *word;
	int	fd;
	int	i = 0;

	if (!(fd = open(path, O_RDONLY)) || !(r->points = (float **)malloc(sizeof(float *) * 51)))
		return (NULL);
	while (get_next_word(fd, &word, " \n\t") > 0)
	{
		if (word && checkw(word))
		{
			r->points[i] = (float *)malloc(sizeof(float) * 3);
			r->points[i][0] = (float)ft_atoi(ft_strchr(word, '(') + 1) / 2000;
			r->points[i][1] = (float)ft_atoi(ft_strchr(word, ',') + 1) / 2000;
			r->points[i++][2] = (float)ft_atoi(ft_strrchr(word, ',') + 1) / 2000;
			free(word);
		}
	}
	r->points[i] = NULL;
	r->size = i;
	return (r->points);
}
