/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 02:03:42 by grhaddad          #+#    #+#             */
/*   Updated: 2026/09/16 11:09:25 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_is_valid_identifier(const char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_error(t_shell *shell, const char *arg)
{
	write(STDERR_FILENO, "minishell: export: `", 20);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
	shell->last_status = 1;
	return (1);
}

static int	add_key_if_missing(t_shell *shell, const char *key)
{
	char	*entry;

	if (ft_env_index(shell->env, key) >= 0)
		return (0);
	entry = ft_strdup(key);
	if (!entry)
		return (1);
	if (add_new_env(shell, entry))
		return (free(entry), 1);
	return (0);
}

static int	process_export_arg(t_shell *shell, char *arg)
{
	char	*eq;
	char	*key;
	int		status;

	if (!ft_is_valid_identifier(arg))
		return (export_error(shell, arg));
	eq = ft_strchr(arg, '=');
	if (!eq)
		return (add_key_if_missing(shell, arg));
	key = ft_substr(arg, 0, eq - arg);
	if (!key)
		return (1);
	status = set_env_val(shell, key, eq + 1);
	free(key);
	return (status);
}

int	export_cmd(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	err;

	if (!shell || !cmd)
		return (1);
	if (!cmd->args[1])
		return (print_sorted_export(shell));
	i = 1;
	err = 0;
	while (cmd->args[i])
	{
		if (process_export_arg(shell, cmd->args[i]) != 0)
			err = 1;
		i++;
	}
	if (!err)
		shell->last_status = 0;
	return (err);
}
