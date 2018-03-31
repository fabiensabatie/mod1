#include "libft.h"

int	ft_strappend(char **s1, char *s2)
{
	char *str;

	if (!s1)
		return (-1);
	if (!(str = ft_strjoin(*s1, s2)))
		return (-1);
	if (*s1)
		free(*s1);
	*s1 = str;
	return (1);
}
