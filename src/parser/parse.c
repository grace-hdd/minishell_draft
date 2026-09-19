/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:44:39 by grhaddad          #+#    #+#             */
/*   Updated: 2026/02/23 20:44:39 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	ft_free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->delimiter);
		free(redir->file);
		free(redir);
		redir = tmp;
	}
}

void	ft_free_cmds(t_cmd *head)
{
	t_cmd	*tmp;

	while (head)
	{
		tmp = head->next;
		ft_free_args(head->args);
		ft_free_redirs(head->redirs);
		free(head);
		head = tmp;
	}
}

t_cmd	*ft_parse(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*cmd;

	head = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
			return (ft_free_cmds(head), ft_syntax_error(tokens->value), NULL);
		cmd = ft_parse_cmd(&tokens);
		if (!cmd)
			return (ft_free_cmds(head), NULL);
		ft_cmd_add_back(&head, cmd);
		if (tokens && tokens->type == TOKEN_PIPE)
		{
			if (!tokens->next)
				return (ft_free_cmds(head), ft_syntax_error(NULL), NULL);
			tokens = tokens->next;
		}
	}
	return (head);
}
