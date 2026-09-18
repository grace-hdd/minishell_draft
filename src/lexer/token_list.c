/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 23:46:48 by grhaddad          #+#    #+#             */
/*   Updated: 2026/02/21 23:46:48 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_token_add_back(t_token **head, t_token *current)
{
	t_token	*tok;

	if (head == NULL || *head == NULL)
	{
		*head = current;
		return ;
	}
	tok = *head;
	while (tok->next != NULL)
		tok = tok->next;
	tok->next = current;
}

void	ft_free_tokens(t_token *head)
{
	t_token	*tok;

	while (head != NULL)
	{
		tok = head;
		head = head->next;
		free(tok->value);
		free(tok);
	}
}
