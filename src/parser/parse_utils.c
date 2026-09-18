/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 21:09:37 by grhaddad          #+#    #+#             */
/*   Updated: 2026/02/23 21:09:37 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_is_redir(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND);
}

static char	**ft_grow_args(char **args, int *len)
{
	char	**new_args;
	int		i;

	*len = 0;
	while (args && args[*len])
		(*len)++;
	new_args = malloc(sizeof(char *) * (*len + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < *len)
	{
		new_args[i] = args[i];
		i++;
	}
	return (new_args);
}

int	ft_args_add_back(t_cmd *cmd, char *value)
{
	char	**new_args;
	int		len;

	if (!cmd)
		return (1);
	new_args = ft_grow_args(cmd->args, &len);
	if (!new_args)
		return (ft_error_msg("allocation failure"), 1);
	new_args[len] = ft_strdup(value);
	if (!new_args[len])
	{
		free(new_args);
		return (ft_error_msg("allocation failure"), 1);
	}
	new_args[len + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	return (0);
}

void	ft_cmd_add_back(t_cmd **head, t_cmd *cmd)
{
	t_cmd	*last;

	if (!head || !cmd)
		return ;
	cmd->next = NULL;
	if (*head == NULL)
	{
		*head = cmd;
		return ;
	}
	last = *head;
	while (last->next != NULL)
		last = last->next;
	last->next = cmd;
}
