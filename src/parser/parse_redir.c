/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 17:04:56 by grhaddad          #+#    #+#             */
/*   Updated: 2026/09/18 19:52:10 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <fcntl.h>

static int	ft_init_heredoc_node(t_redir **new_node, const char *target)
{
	*new_node = malloc(sizeof(t_redir));
	if (!*new_node)
	{
		ft_error_msg("allocation failure");
		return (1);
	}
	(*new_node)->type = TOKEN_HEREDOC;
	(*new_node)->file = NULL;
	(*new_node)->fd = -1;
	(*new_node)->delimiter = ft_hd_delimiter((char *)target,
			&(*new_node)->heredoc_expand);
	if (!(*new_node)->delimiter)
	{
		free(*new_node);
		ft_error_msg("allocation failure");
		return (1);
	}
	(*new_node)->next = NULL;
	return (0);
}

static int	ft_init_redir_node(t_redir **new_node, int type,
		const char *target)
{
	*new_node = malloc(sizeof(t_redir));
	if (!*new_node)
	{
		ft_error_msg("allocation failure");
		return (1);
	}
	(*new_node)->type = type;
	(*new_node)->file = ft_strdup(target);
	(*new_node)->delimiter = NULL;
	(*new_node)->heredoc_expand = 0;
	(*new_node)->fd = -1;
	if (!(*new_node)->file)
	{
		free(*new_node);
		ft_error_msg("allocation failure");
		return (1);
	}
	return (0);
}

static int	ft_is_valid_target(t_token *target)
{
	if (!target)
		return (0);
	if (target->type == TOKEN_PIPE || ft_is_redir(target->type))
		return (0);
	return (1);
}

static int	ft_add_redir_node(t_cmd *cmd, int type, const char *target)
{
	t_redir	*new_node;
	int		status;

	if (type == TOKEN_HEREDOC)
		status = ft_init_heredoc_node(&new_node, target);
	else
		status = ft_init_redir_node(&new_node, type, target);
	if (status)
		return (1);
	new_node->next = NULL;
	ft_redir_add_back(cmd, new_node);
	return (0);
}

int	ft_parse_redir(t_cmd *cmd, t_token **tokens)
{
	int		redir_type;
	t_token	*target;
	char	*err_val;

	if (!cmd || !tokens || !*tokens)
		return (1);
	while (*tokens && ft_is_redir((*tokens)->type))
	{
		redir_type = (*tokens)->type;
		*tokens = (*tokens)->next;
		target = *tokens;
		if (!ft_is_valid_target(target))
		{
			err_val = NULL;
			if (target)
				err_val = target->value;
			ft_syntax_error(err_val);
			return (1);
		}
		if (ft_add_redir_node(cmd, redir_type, target->value))
			return (1);
		*tokens = target->next;
	}
	return (0);
}
