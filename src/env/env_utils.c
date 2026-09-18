/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 01:36:45 by grhaddad          #+#    #+#             */
/*   Updated: 2026/09/16 11:11:46 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_env_val(t_shell *shell, const char *key)
{
	int		i;
	size_t	key_len;

	if (!shell || !shell->env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, key_len) == 0
			&& shell->env[i][key_len] == '=')
			return (shell->env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

static char	*create_env_str(const char *key, const char *value)
{
	char	*tmp;
	char	*str;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	str = ft_strjoin(tmp, value);
	free(tmp);
	return (str);
}

int	add_new_env(t_shell *shell, char *new_entry)
{
	int		i;
	char	**new_env;

	i = 0;
	while (shell->env && shell->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	i = -1;
	while (shell->env && shell->env[++i])
		new_env[i] = shell->env[i];
	new_env[i] = new_entry;
	new_env[i + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
	return (0);
}

static int	update_existing_env(t_shell *shell, const char *key,
		size_t key_len, char *new_entry)
{
	int		i;

	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, key_len) == 0
			&& (shell->env[i][key_len] == '=' 
			|| shell->env[i][key_len] == '\0'))
		{
			free(shell->env[i]);
			shell->env[i] = new_entry;
			return (1);
		}
		i++;
	}
	return (0);
}

int	set_env_val(t_shell *shell, const char *key, const char *value)
{
	size_t	key_len;
	char	*new_entry;

	if (!shell || !key || !value)
		return (1);
	new_entry = create_env_str(key, value);
	if (!new_entry)
		return (1);
	key_len = ft_strlen(key);
	if (update_existing_env(shell, key, key_len, new_entry))
		return (0);
	if (add_new_env(shell, new_entry))
	{
		free(new_entry);
		return (1);
	}
	return (0);
}
