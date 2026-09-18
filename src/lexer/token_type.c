/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:04:42 by grhaddad          #+#    #+#             */
/*   Updated: 2026/02/21 00:04:42 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_token	*ft_get_double_operator_token(char *input, int *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		*i += 2;
		return (ft_new_token(TOKEN_HEREDOC, "<<"));
	}
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		*i += 2;
		return (ft_new_token(TOKEN_APPEND, ">>"));
	}
	return (NULL);
}

t_token	*ft_get_operator_token(char *input, int *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
		return (ft_get_double_operator_token(input, i));
	if (input[*i] == '>' && input[*i + 1] == '>')
		return (ft_get_double_operator_token(input, i));
	if (input[*i] == '<')
	{
		(*i)++;
		return (ft_new_token(TOKEN_REDIR_IN, "<"));
	}
	if (input[*i] == '>')
	{
		(*i)++;
		return (ft_new_token(TOKEN_REDIR_OUT, ">"));
	}
	if (input[*i] == '|')
	{
		(*i)++;
		return (ft_new_token(TOKEN_PIPE, "|"));
	}
	return (NULL);
}
