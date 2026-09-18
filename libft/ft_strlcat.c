/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysarrouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 17:08:17 by ysarrouh          #+#    #+#             */
/*   Updated: 2025/06/12 21:43:53 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dst[j] != '\0' && j < size)
		j++;
	while (src[i] != '\0' && (j + i + 1) < size)
	{
		dst[j + i] = src[i];
		i++;
	}
	if (j < size)
		dst[j + i] = '\0';
	while (src[i] != '\0')
		i++;
	return (j + i);
}
/*#include <stdio.h>
int	main(void)
{
	char dst[30] = "yendy";
	const char *src = "sarrouh";
	size_t size = 15;

	printf("%ld", ft_strlcat(dst, src, size));
	return (0);
}*/
