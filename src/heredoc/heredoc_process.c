/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 13:48:43 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/09/18 20:10:07 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_wait_heredoc(pid_t pid)
{
	int	status;
	int	result;

	result = waitpid(pid, &status, 0);
	while (result < 0 && errno == EINTR)
		result = waitpid(pid, &status, 0);
	if (result < 0)
		return (1);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (130);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static int	ft_open_one_heredoc(t_redir *redir)
{
	redir->fd = open(redir->file, O_RDONLY);
	if (redir->fd < 0)
		return (perror("minishell: heredoc"), 1);
	unlink(redir->file);
	return (0);
}

static int	ft_open_heredocs(t_cmd *cmd)
{
	t_redir	*redir;

	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC && redir->fd < 0
				&& ft_open_one_heredoc(redir))
				return (1);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

int	ft_run_heredocs(t_shell *shell)
{
	pid_t			pid;
	int				status;

	ft_signals_exec();
	pid = fork();
	if (pid < 0)
	{
		setup_signals();
		return (1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		rl_catch_signals = 1;
		ft_shell_exit(shell, ft_collect_heredocs(shell));
	}
	status = ft_wait_heredoc(pid);
	setup_signals();
	if (status == 0)
		status = ft_open_heredocs(shell->cmds);
	return (status);
}
