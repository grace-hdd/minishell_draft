/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysarrouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:04:45 by ysarrouh          #+#    #+#             */
/*   Updated: 2025/06/12 21:23:17 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	const unsigned char	*sr;

	sr = (const unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (sr[i] == (unsigned char)c)
		{
			return ((void *)&sr[i]);
		}
		i++;
	}
	return (NULL);
}
/*#include <stdio.h>
int main(void)
{
    char str[] = "yendy";
    char *result = ft_memchr(str, 'n', 5);

    if (result)
        printf("Found: %s\n", result);
    else
        printf("Not found\n");

    return (0);
}*/
