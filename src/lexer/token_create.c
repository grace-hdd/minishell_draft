/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:52:07 by grhaddad          #+#    #+#             */
/*   Updated: 2026/02/21 00:52:07 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_token	*ft_new_token(t_token_type type, char *value)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->next = NULL;
	return (token);
}

t_token	*ft_get_word_token(char *input, int *i)
{
	int		start;
	char	*word;
	t_token	*tok;

	start = *i;
	while (input[*i] && !ft_is_stop_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (ft_skip_quoted_sequence(input, i))
				return (NULL);
		}
		else
			(*i)++;
	}
	word = ft_substr(input, start, *i - start);
	if (!word)
		return (ft_error_msg("allocation failure"), NULL);
	tok = ft_new_token(TOKEN_WORD, word);
	free(word);
	if (!tok)
		return (ft_error_msg("allocation failure"), NULL);
	return (tok);
}
