/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 09:44:06 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/08/26 20:05:37 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_close_fd(int *fd)
{
	if (*fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

static int	ft_status_from_wait(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		return (128 + sig);
	}
	return (1);
}

int	ft_wait_all(pid_t last_pid, int count)
{
	pid_t	pid;
	int		status;
	int		result;

	result = 1;
	while (count > 0)
	{
		pid = waitpid(-1, &status, 0);
		if (pid < 0 && errno == EINTR)
			continue ;
		if (pid < 0)
			break ;
		if (pid == last_pid)
			result = ft_status_from_wait(status);
		count--;
	}
	return (result);
}
