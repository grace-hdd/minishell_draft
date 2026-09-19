/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 14:53:00 by grhaddad          #+#    #+#             */
/*   Updated: 2026/07/30 14:53:00 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_validate_quotes(char *input)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (input && input[i])
	{
		ft_update_quote(input[i], &quote);
		i++;
	}
	if (quote)
		return (ft_unclosed_quote_error());
	return (0);
}

static int	ft_validate_pipe(t_token *prev, t_token *cur)
{
	if (!prev)
		return (ft_syntax_error(cur->value), 2);
	if (!cur->next)
		return (ft_syntax_error(NULL), 2);
	if (prev->type == TOKEN_PIPE || ft_is_redir(prev->type))
		return (ft_syntax_error(cur->value), 2);
	if (cur->next->type == TOKEN_PIPE)
		return (ft_syntax_error(cur->next->value), 2);
	return (0);
}

static int	ft_validate_redir(t_token *cur)
{
	if (!cur->next)
		return (ft_syntax_error(NULL), 2);
	if (cur->next->type == TOKEN_PIPE || ft_is_redir(cur->next->type))
		return (ft_syntax_error(cur->next->value), 2);
	return (0);
}

int	ft_validate_syntax(t_token *tokens)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE && ft_validate_pipe(prev, tokens))
			return (2);
		if (ft_is_redir(tokens->type) && ft_validate_redir(tokens))
			return (2);
		prev = tokens;
		tokens = tokens->next;
	}
	return (0);
}
