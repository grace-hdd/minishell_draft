/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysarrouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 00:25:22 by ysarrouh          #+#    #+#             */
/*   Updated: 2025/06/25 22:18:58 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*static char ft_f(unsigned int i, char c)
{
	if (i % 2 == 0)
	{
		if (c >= 'A' && c <= 'Z')
			return (c + 32);
		else if (c >= 'a' && c <= 'z')
			return (c - 32);
	}
	return (c);
}*/
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*res;

	res = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		res[i] = f(i, s[i]);
		i++;
	}
	res[i] = '\0';
	return (res);
}

/*#include <stdio.h>
int	main(void)
{
	char const *s = "yendy";

	printf("%s", ft_strmapi(s, ft_f));
	return (0);
}*/
