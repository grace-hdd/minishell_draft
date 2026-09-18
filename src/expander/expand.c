/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 15:59:53 by grhaddad          #+#    #+#             */
/*   Updated: 2026/09/16 09:59:03 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_expand_redirs(t_redir *redir, t_shell *shell)
{
	char	*expanded;
	char	*old;

	while (redir)
	{
		if (redir->type != TOKEN_HEREDOC)
		{
			old = redir->file;
			expanded = ft_expand_str(redir->file, shell);
			if (expanded)
			{
				redir->file = expanded;
				free(old);
			}
		}
		redir = redir->next;
	}
}

static int	ft_has_quote(const char *str)
{
	while (str && *str)
	{
		if (*str == '\'' || *str == '"')
			return (1);
		str++;
	}
	return (0);
}

static void	ft_remove_arg(char **args, int i)
{
	free(args[i]);
	while (args[i])
	{
		args[i] = args[i + 1];
		i++;
	}
}

static void	ft_expand_args(char **args, t_shell *shell)
{
	char	*expanded;
	int		i;

	i = 0;
	while (args && args[i])
	{
		expanded = ft_expand_str(args[i], shell);
		if (expanded && expanded[0] == '\0' && !ft_has_quote(args[i]))
		{
			free(expanded);
			ft_remove_arg(args, i);
		}
		else
		{
			if (expanded)
			{
				free(args[i]);
				args[i] = expanded;
			}
			i++;
		}
	}
}

void	ft_expand(t_cmd *cmds, t_shell *shell)
{
	while (cmds)
	{
		ft_expand_args(cmds->args, shell);
		ft_expand_redirs(cmds->redirs, shell);
		cmds = cmds->next;
	}
}
