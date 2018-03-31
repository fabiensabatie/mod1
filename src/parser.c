#include "mod1.h"

float	*parser(char *path)
{
	char *line;
	int	fd;
	int	ret;

	if (!(fd = open(path, O_RDONLY)))
		return (NULL);
	while ((ret = get_next_line(fd, &line)) > 0)
	{

	}
	if (ret < 0)
		return (NULL);
}
