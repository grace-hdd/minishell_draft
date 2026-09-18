/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 09:52:11 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/08/26 21:39:24 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_child_routine(t_shell *shell, t_cmd *cmd, int prev_fd, int *pfd)
{
	int	status;

	ft_signals_child();
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
	}
	status = ft_execute_child_cmd(shell, cmd);
	ft_child_exit(shell, NULL, status);
}

static int	ft_fork_one(t_shell *shell, t_cmd *cmd, int prev_fd, pid_t *pid)
{
	int	pfd[2];

	pfd[0] = -1;
	pfd[1] = -1;
	if (cmd->next && pipe(pfd) < 0)
		return (ft_error_msg("pipe failed"), -2);
	*pid = fork();
	if (*pid < 0)
	{
		ft_error_msg("fork failed");
		ft_close_fd(&pfd[0]);
		ft_close_fd(&pfd[1]);
		return (-2);
	}
	if (*pid == 0)
		ft_child_routine(shell, cmd, prev_fd, pfd);
	ft_close_fd(&prev_fd);
	ft_close_fd(&pfd[1]);
	return (pfd[0]);
}

static int	ft_fork_commands(t_shell *shell, t_cmd *cmd, t_pipe_state *state)
{
	int	count;
	int	next_fd;

	count = 0;
	while (cmd)
	{
		next_fd = ft_fork_one(shell, cmd, *state->prev_fd, state->last_pid);
		if (next_fd == -2)
		{
			*state->failed = 1;
			break ;
		}
		*state->prev_fd = next_fd;
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	ft_execute_pipeline(t_shell *shell)
{
	pid_t	last_pid;
	int		prev_fd;
	int		count;
	int		failed;

	ft_signals_exec();
	prev_fd = -1;
	last_pid = -1;
	failed = 0;
	count = ft_fork_commands(shell, shell->cmds,
			&(t_pipe_state){&prev_fd, &last_pid, &failed});
	ft_close_fd(&prev_fd);
	if (count > 0)
		count = ft_wait_all(last_pid, count);
	setup_signals();
	if (failed)
		return (1);
	return (count);
}
