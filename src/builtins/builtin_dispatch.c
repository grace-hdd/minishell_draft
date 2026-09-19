/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 15:19:22 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/08/26 20:25:44 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_builtin_type	ft_builtin_type(char *cmd)
{
	if (cmd == NULL)
		return (BUILTIN_NONE);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (BUILTIN_ECHO);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (BUILTIN_CD);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (BUILTIN_PWD);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (BUILTIN_EXPORT);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (BUILTIN_UNSET);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (BUILTIN_ENV);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (BUILTIN_EXIT);
	return (BUILTIN_NONE);
}

int	ft_execute_builtin(t_shell *shell, t_cmd *cmd, t_builtin_type type)
{
	if (type == BUILTIN_ECHO)
		return (echo_cmd(shell, cmd));
	if (type == BUILTIN_CD)
		return (cd_cmd(shell, cmd));
	if (type == BUILTIN_PWD)
		return (pwd_cmd(shell, cmd));
	if (type == BUILTIN_EXPORT)
		return (export_cmd(shell, cmd));
	if (type == BUILTIN_UNSET)
		return (unset_cmd(shell, cmd));
	if (type == BUILTIN_ENV)
		return (env_cmd(shell, cmd));
	if (type == BUILTIN_EXIT)
		return (exit_cmd(shell, cmd));
	return (1);
}
