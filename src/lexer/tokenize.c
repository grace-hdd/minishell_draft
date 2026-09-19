/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:05:00 by grhaddad          #+#    #+#             */
/*   Updated: 2026/02/21 00:05:00 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_token	*ft_get_next_token(char *input, int *i)
{
	if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
		return (ft_get_operator_token(input, i));
	return (ft_get_word_token(input, i));
}

t_token	*ft_tokenize(char *input)
{
	t_token	*head;
	t_token	*tok;
	int		i;

	i = 0;
	head = NULL;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == '\0')
			break ;
		tok = ft_get_next_token(input, &i);
		if (tok == NULL)
		{
			ft_free_tokens(head);
			return (NULL);
		}
		ft_token_add_back(&head, tok);
	}
	return (head);
}
