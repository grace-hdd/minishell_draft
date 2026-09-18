/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysarrouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 22:19:16 by ysarrouh          #+#    #+#             */
/*   Updated: 2025/06/26 20:42:05 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_wordcounter(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			count++;
		i++;
	}
	return (count);
}

static char	*ft_malloc(const char *s, int start, int end)
{
	char	*word;
	int		i;

	word = (char *)malloc((end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (start < end)
	{
		word[i] = s[start];
		i++;
		start++;
	}
	word[i] = '\0';
	return (word);
}

static void	ft_free(char **res, int j)
{
	while (j--)
		free (res[j]);
	free(res);
}

static int	ft_result(char **res, const char *s, char c)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
		{
			start = i;
			while (s[i] != '\0' && s[i] != c)
				i++;
			res[j] = ft_malloc(s, start, i);
			if (!res[j])
				return (ft_free(res, j), 0);
			j++;
		}
	}
	res[j] = NULL;
	return (1);
}

char	**ft_split(const char *s, char c)
{
	int			wc;
	char		**res;

	if (!s)
		return (NULL);
	wc = ft_wordcounter(s, c);
	res = (char **)malloc((wc + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	if (!ft_result(res, s, c))
		return (NULL);
	return (res);
}
/*#include <stdio.h>
int	main(void)
{
	const char *s = "yendynd42";
	char		c = 'd';
	char	**final = ft_split(s, c);
	int			i = 0;

	if (!final)
	{
		printf("failed");
		return (1);
	}
	while (final[i])
	{
		printf("word[%d]: %s\n", i, final[i]);
		free(final[i]);
		i++;
	}
	free(final);
	}*/
