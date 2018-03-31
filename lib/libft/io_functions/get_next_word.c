#include "libft.h"
# define W_SIZE 1000

int	get_next_word(int fd, char **word, char *separator)
{
	int			ret;
	char 		*buf;
	static char	*save;

	if (fd < 0 || W_SIZE < 1 || read(fd, "", 0) < 0 || !(buf = ft_strnew(BUFF_SIZE)) || !separator)
		return (-1);
	while ((ret = read(fd, buf, W_SIZE)))
	{
		buf[ret] = '\0';
		if (!ft_strappend(&save, buf))
			return (-1);
		if (ft_strpbrk(buf, separator))
			break ;
	}
	free(buf);
	if (ret < BUFF_SIZE && (!save || !ft_strlen(save)))
		return (0);
	return (1);
}
