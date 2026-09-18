/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 19:21:04 by grhaddad          #+#    #+#             */
/*   Updated: 2026/07/27 19:21:04 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

static void	sort_env_array(char **env, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export_line(const char *str)
{
	char	*equal_sign;

	write(STDOUT_FILENO, "declare -x ", 11);
	equal_sign = ft_strchr(str, '=');
	if (equal_sign)
	{
		write(STDOUT_FILENO, str, equal_sign - str + 1);
		write(STDOUT_FILENO, "\"", 1);
		write(STDOUT_FILENO, equal_sign + 1, ft_strlen(equal_sign + 1));
		write(STDOUT_FILENO, "\"\n", 2);
	}
	else
	{
		write(STDOUT_FILENO, str, ft_strlen(str));
		write(STDOUT_FILENO, "\n", 1);
	}
}

static char	**dup_env_array(char **env, int *count)
{
	char	**dup;
	int		i;

	*count = 0;
	while (env && env[*count])
		(*count)++;
	dup = malloc(sizeof(char *) * (*count + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < *count)
	{
		dup[i] = ft_strdup(env[i]);
		if (!dup[i])
		{
			while (i > 0)
				free(dup[--i]);
			free(dup);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}

int	print_sorted_export(t_shell *shell)
{
	char	**dup;
	int		count;
	int		i;

	if (!shell || !shell->env)
		return (0);
	dup = dup_env_array(shell->env, &count);
	if (!dup)
		return (1);
	sort_env_array(dup, count);
	i = 0;
	while (i < count)
	{
		print_export_line(dup[i]);
		free(dup[i]);
		i++;
	}
	free(dup);
	return (0);
}
