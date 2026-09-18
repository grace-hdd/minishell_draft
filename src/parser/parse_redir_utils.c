/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 16:20:42 by grhaddad          #+#    #+#             */
/*   Updated: 2026/07/30 16:20:42 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	ft_redir_add_back(t_cmd *cmd, t_redir *new_node)
{
	t_redir	*last;

	if (!cmd->redirs)
	{
		cmd->redirs = new_node;
		return ;
	}
	last = cmd->redirs;
	while (last->next)
		last = last->next;
	last->next = new_node;
}
