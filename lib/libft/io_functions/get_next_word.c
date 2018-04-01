#include "libft.h"
# define W_SIZE 1000
#include <stdio.h>

t_list	*ft_lstfind(t_list **list, int fd)
{
	t_list	*tmp;

	tmp = *list;
	while (tmp)
	{
		if ((int)tmp->content_size == fd)
			break ;
		tmp = tmp->next;
	}
	if (!tmp)
	{
		ft_lstadd(list, ft_lstnew(NULL, fd));
		(*list)->content_size = fd;
		tmp = *list;
	}
	return (tmp);
}

int	get_next_word(int fd, char **word, char *separator)
{
	int				ret;
	char 			*buf;
	static t_list	*list = NULL;
	t_list			*current;

	if (fd < 0 || W_SIZE < 1 || read(fd, "", 0) < 0 || !(buf = ft_strnew(W_SIZE)) || !separator)
		return (-1);
	current = ft_lstfind(&list, fd);
	while ((ret = read(fd, buf, W_SIZE)))
	{
		buf[ret] = '\0';
		current->content = (current->content)
		? ft_strjoinfree((char *)current->content, buf) : ft_strdup(buf);
		if (ft_strpbrk(buf, separator))
			break ;
	}
	if (buf)
		free(buf);
	if (!current->content || (ret < W_SIZE && !ft_strlen(current->content)))
		return (0);
	*word = (ft_strpbrk(current->content, separator)) ? ft_strdupchr(current->content, *ft_strpbrk(current->content, separator))
	: ft_strdup(current->content);
	(ft_strlen(*word) < ft_strlen(current->content)) ?
	ft_strcpy(current->content, ft_strpbrk(current->content, separator) + 1)
	: ft_strclr(current->content);
	return (1);
}
