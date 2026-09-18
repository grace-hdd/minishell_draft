/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 20:54:58 by grhaddad          #+#    #+#             */
/*   Updated: 2026/02/23 20:54:58 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_free_one_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	ft_free_args(cmd->args);
	ft_free_redirs(cmd->redirs);
	free(cmd);
}

static int	ft_parse_cmd_token(t_cmd *cmd, t_token **tokens)
{
	if ((*tokens)->type == TOKEN_WORD)
	{
		if (ft_args_add_back(cmd, (*tokens)->value))
			return (1);
		*tokens = (*tokens)->next;
		return (0);
	}
	if (ft_is_redir((*tokens)->type))
		return (ft_parse_redir(cmd, tokens));
	ft_syntax_error((*tokens)->value);
	return (1);
}

t_cmd	*ft_parse_cmd(t_token **tokens)
{
	t_cmd	*cmd;

	if (!tokens || !*tokens)
		return (NULL);
	if ((*tokens)->type == TOKEN_PIPE)
		return (ft_syntax_error((*tokens)->value), NULL);
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (ft_error_msg("allocation failure"), NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if (ft_parse_cmd_token(cmd, tokens))
			return (ft_free_one_cmd(cmd), NULL);
	}
	return (cmd);
}
