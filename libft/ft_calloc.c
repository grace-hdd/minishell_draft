/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysarrouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 22:59:53 by ysarrouh          #+#    #+#             */
/*   Updated: 2025/06/26 20:19:18 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total;

	if (nmemb == 0 || size == 0)
	{
		ptr = malloc(1);
		if (ptr)
			ft_bzero(ptr, 1);
		return (ptr);
	}
	if (nmemb > SIZE_MAX / size)
		return (NULL);
	total = nmemb * size;
	if (total < nmemb || total < size)
		return (NULL);
	ptr = malloc(total);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total);
	return (ptr);
}

/*#include <stdio.h>
int	main(void)
{
	int	*arr = ft_calloc(3, sizeof(int));

	if (arr)
		printf("%d %d %d", arr[0], arr[1], arr[2]);

	free(arr);
	return (0);
}*/
