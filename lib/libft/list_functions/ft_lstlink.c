#include "libft.h"

t_list	*ft_lstlink(void *content, size_t content_size, size_t size)
{
	t_list	*list;

	if (!(list = (t_list *)ft_memalloc(sizeof(t_list))))
		return (NULL);
	list->content = content;
	(void)size;
	list->content_size = content_size;
	list->next = NULL;
	return (list);
}
