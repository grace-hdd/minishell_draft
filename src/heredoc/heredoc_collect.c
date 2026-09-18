/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 13:33:41 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/08/26 21:33:46 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_hd_write_line(int fd, char *line, t_redir *redir, t_shell *shell)
{
	char	*output;
	int		failed;

	output = line;
	if (redir->heredoc_expand)
		output = ft_expand_heredoc(line, shell);
	if (!output)
		return (1);
	failed = ft_hd_write_all(fd, output);
	if (!failed)
		failed = ft_hd_write_all(fd, "\n");
	if (output != line)
		free(output);
	return (failed);
}

static int	ft_collect_lines(int fd, t_redir *redir, t_shell *shell);

static int	ft_hd_eof(char **line)
{
	*line = readline("> ");
	if (!*line)
	{
		ft_putendl_fd("minishell: warning: here-document "
			"delimited by end-of-file", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int	ft_collect_one(t_redir *redir, t_shell *shell)
{
	int		fd;

	fd = open(redir->file, O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
		return (perror("minishell: heredoc"), 1);
	return (ft_collect_lines(fd, redir, shell));
}

static int	ft_collect_lines(int fd, t_redir *redir, t_shell *shell)
{
	char	*line;
	int		failed;

	failed = 0;
	while (!failed)
	{
		if (ft_hd_eof(&line))
			break ;
		if (!ft_strncmp(line, redir->delimiter,
				ft_strlen(redir->delimiter) + 1))
		{
			free(line);
			close(fd);
			return (0);
		}
		failed = ft_hd_write_line(fd, line, redir, shell);
		free(line);
	}
	close(fd);
	return (failed);
}

int	ft_collect_heredocs(t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = shell->cmds;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC
				&& ft_collect_one(redir, shell))
				return (1);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
