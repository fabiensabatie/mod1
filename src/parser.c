#include "mod1.h"

t_list	*parser(char *path)
{
	t_list	*list;
	char *line;
	int	fd;
	int	ret;
	int	i = 0;

	list = NULL;
	if (!(fd = open(path, O_RDONLY)))
		return (NULL);
	while ((ret = get_next_word(fd, &line, " \n\t")) > 0)
	{
		ft_lstadd(&list, ft_lstnew(line, sizeof(*line)));
		if (line)
		{
			ft_putnbr(i++);
			ft_putendl(line);
		}
	}
	return (list);
}
