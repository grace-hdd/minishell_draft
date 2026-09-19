/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/26 10:10:34 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/09/16 09:59:32 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_execute(t_shell *shell)
{
	t_cmd			*cmd;
	t_builtin_type	type;

	cmd = shell->cmds;
	if (!cmd)
		return ;
	if (cmd->next == NULL && (!cmd->args || !cmd->args[0]))
	{
		shell->last_status = ft_execute_redir_only(cmd);
		return ;
	}
	type = BUILTIN_NONE;
	if (cmd->args && cmd->args[0])
		type = ft_builtin_type(cmd->args[0]);
	if (type != BUILTIN_NONE && cmd->next == NULL)
	{
		shell->last_status = ft_execute_parent_builtin(shell, cmd, type);
		return ;
	}
	shell->last_status = ft_execute_pipeline(shell);
}

int	ft_execute_child_cmd(t_shell *shell, t_cmd *cmd)
{
	t_builtin_type	type;

	if (!cmd)
		return (1);
	if (ft_redirection(cmd->redirs))
		return (1);
	if (!cmd->args || !cmd->args[0])
		return (0);
	type = ft_builtin_type(cmd->args[0]);
	if (type != BUILTIN_NONE)
		return (ft_execute_builtin(shell, cmd, type));
	return (ft_execute_external(shell, cmd));
}
