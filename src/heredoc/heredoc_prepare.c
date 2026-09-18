/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_prepare.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 09:20:25 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/09/18 20:09:26 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_has_heredoc(t_cmd *cmd)
{
	t_redir	*redir;

	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
				return (1);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

static int	ft_assign_paths(t_cmd *cmd, int *counter)
{
	t_redir	*redir;

	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				redir->file = ft_hd_temp_path(counter);
				if (!redir->file)
					return (1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

int	ft_prepare_heredocs(t_shell *shell)
{
	int	counter;

	if (!ft_has_heredoc(shell->cmds))
		return (0);
	counter = 0;
	if (ft_assign_paths(shell->cmds, &counter))
	{
		ft_discard_heredocs(shell->cmds);
		ft_error_msg("allocation failure");
		shell->last_status = 1;
		return (1);
	}
	shell->last_status = ft_run_heredocs(shell);
	if (shell->last_status != 0)
	{
		ft_discard_heredocs(shell->cmds);
		return (1);
	}
	return (0);
}

void	ft_cleanup_heredocs(t_cmd *cmds)
{
	t_redir	*redir;

	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC && redir->fd >= 0)
			{
				close(redir->fd);
				redir->fd = -1;
			}
			redir = redir->next;
		}
		cmds = cmds->next;
	}
}

void	ft_discard_heredocs(t_cmd *cmds)
{
	t_redir	*redir;

	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC && redir->file)
				unlink(redir->file);
			redir = redir->next;
		}
		cmds = cmds->next;
	}
}
