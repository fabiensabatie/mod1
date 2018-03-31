#include "libft.h"

char	*ft_strdupchr(char *src, char c)
{
	char	*str;

	if (!src || !(str = ft_strndup(src, ft_strchr(src, c) - src)))
		return (NULL);
	return (str);
}
