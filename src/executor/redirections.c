/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 11:00:51 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/09/18 20:05:38 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_open_redirection(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == TOKEN_HEREDOC)
	{
		fd = redir->fd;
		redir->fd = -1;
	}
	else if (redir->type == TOKEN_REDIR_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == TOKEN_REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (redir->type == TOKEN_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0666);
	else
		errno = EINVAL;
	return (fd);
}

static int	ft_redirection_error(char *file)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(file);
	return (1);
}

static int	ft_dup_redirection(int fd, int target, char *file)
{
	int	saved_errno;

	if (dup2(fd, target) < 0)
	{
		saved_errno = errno;
		close(fd);
		errno = saved_errno;
		return (ft_redirection_error(file));
	}
	if (fd != target)
		close(fd);
	return (0);
}

int	ft_redirection(t_redir *redirs)
{
	int	fd;
	int	target;

	while (redirs)
	{
		fd = ft_open_redirection(redirs);
		if (fd < 0)
			return (ft_print_errno(redirs->file, 1));
		target = STDIN_FILENO;
		if (redirs->type == TOKEN_REDIR_OUT || redirs->type == TOKEN_APPEND)
			target = STDOUT_FILENO;
		if (ft_dup_redirection(fd, target, redirs->file))
			return (1);
		redirs = redirs->next;
	}
	return (0);
}
