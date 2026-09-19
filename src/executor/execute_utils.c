/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/25 17:39:42 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/08/26 20:05:37 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_save_stdio(int *stdin_fd, int *stdout_fd)
{
	*stdin_fd = dup(STDIN_FILENO);
	if (*stdin_fd < 0)
		return (1);
	*stdout_fd = dup(STDOUT_FILENO);
	if (*stdout_fd < 0)
	{
		close(*stdin_fd);
		return (1);
	}
	return (0);
}

static void	ft_restore_stdio(int stdin_fd, int stdout_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
}

int	ft_execute_parent_builtin(t_shell *shell, t_cmd *cmd, t_builtin_type type)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (ft_save_stdio(&saved_stdin, &saved_stdout))
		return (1);
	if (ft_redirection(cmd->redirs))
	{
		ft_restore_stdio(saved_stdin, saved_stdout);
		return (1);
	}
	status = ft_execute_builtin(shell, cmd, type);
	ft_restore_stdio(saved_stdin, saved_stdout);
	return (status);
}

int	ft_execute_redir_only(t_cmd *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	if (ft_save_stdio(&saved_stdin, &saved_stdout))
		return (1);
	status = ft_redirection(cmd->redirs);
	ft_restore_stdio(saved_stdin, saved_stdout);
	return (status);
}
