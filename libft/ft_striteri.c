/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysarrouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 01:04:37 by ysarrouh          #+#    #+#             */
/*   Updated: 2025/06/24 23:57:07 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*static void	ft_f(unsigned int i, char *s)
{
	unsigned int	j;

	j = i;
	if (j % 2 == 0)
	{
		if (s[j] >= 'A' && s[j] <= 'Z')
			s[j] = s[j] + 32;
		else if (s[j] >= 'a' && s[j] <= 'z')
			s[j] = s[j] - 32;
	}
}*/

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}

/*#include <stdio.h>
int	main(void)
{
	char	s[] = "yendy";
	ft_striteri(s, ft_f);
	printf("%s", s);
	return (0);
}*/
