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

static int	unset_single_var(t_shell *shell, const char *key)
{
	int		index;

	if (!shell || !key)
		return (0);
	index = ft_env_index(shell->env, key);
	if (index < 0)
		return (0);
	return (remove_env_var(shell, index));
}

static int	unset_one(t_shell *shell, char *arg)
{
	if (!ft_is_valid_identifier(arg) || ft_strchr(arg, '='))
		return (0);
	return (unset_single_var(shell, arg));
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
		if (unset_one(cmd->args[i]) && unset_single_var(shell, cmd->args[i]))
			err = 1;
		i++;
	}
	shell->last_status = err;
	return (err);
}
