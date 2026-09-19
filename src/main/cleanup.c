/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 09:08:30 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/08/26 20:05:37 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_environment(shell);
	ft_cleanup_heredocs(shell->cmds);
	ft_free_cmds(shell->cmds);
	shell->cmds = NULL;
	free(shell->input);
	shell->input = NULL;
}

void	ft_shell_exit(t_shell *shell, int code)
{
	ft_free_shell(shell);
	rl_clear_history();
	exit((unsigned char)code);
}

void	ft_child_exit(t_shell *shell, char *path, int code)
{
	free(path);
	ft_shell_exit(shell, code);
}
