/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 19:00:35 by grhaddad          #+#    #+#             */
/*   Updated: 2026/09/16 12:50:23 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	remove_env_var(t_shell *shell, int index)
{
	int		i;
	int		len;
	char	**new_env;

	len = 0;
	while (shell->env[len])
		len++;
	new_env = malloc(sizeof(char *) * len);
	if (!new_env)
		return (1);
	i = 0;
	len = 0;
	while (shell->env[i])
	{
		if (i != index)
			new_env[len++] = shell->env[i];
		else
			free(shell->env[i]);
		i++;
	}
	new_env[len] = NULL;
	free(shell->env);
	shell->env = new_env;
	return (0);
}

static int	is_valid_unset_key(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	unset_single_var(t_shell *shell, const char *key)
{
	int		i;
	size_t	key_len;

	if (!shell || !shell->env || !key)
		return (0);
	key_len = ft_strlen(key);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, key_len) == 0
			&& (shell->env[i][key_len] == '='
			|| shell->env[i][key_len] == '\0'))
		{
			return (remove_env_var(shell, i));
		}
		i++;
	}
	return (0);
}

int	unset_cmd(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	err;

	if (!shell || !cmd)
		return (1);
	i = 1;
	err = 0;
	while (cmd->args[i])
	{
		if (is_valid_unset_key(cmd->args[i]) && unset_single_var(shell, cmd->args[i]))
			err = 1;
		i++;
	}
	shell->last_status = err;
	return (err);
}
