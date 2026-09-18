/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysarrouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 23:43:17 by ysarrouh          #+#    #+#             */
/*   Updated: 2025/06/26 00:24:17 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	size_t	s1len;
	size_t	s2len;

	if (!s1 || !s2)
		return (NULL);
	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	join = (char *)malloc(sizeof(char) * (s1len + s2len + 1));
	if (!join)
		return (NULL);
	ft_strlcpy(join, s1, s1len + 1);
	ft_strlcat(join, s2, s1len + s2len + 1);
	return (join);
}
/*#include <stdio.h>
int	main(void)
{
	char const	*s1 = "yendy ";
	char const	*s2 = "sarrouh";

	printf("%s", ft_strjoin(s1, s2));
	return (0);
}*/
