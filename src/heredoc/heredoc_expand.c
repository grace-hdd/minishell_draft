/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 13:09:04 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/08/26 20:24:07 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_hd_append(char **result, char *value)
{
	char	*joined;

	joined = ft_strjoin(*result, value);
	if (!joined)
		return (1);
	free(*result);
	*result = joined;
	return (0);
}

static int	ft_hd_literal(char **result, char c, int *i)
{
	char	value[2];

	value[0] = c;
	value[1] = '\0';
	(*i)++;
	return (ft_hd_append(result, value));
}

static int	ft_hd_status(char **result, int *i, t_shell *shell)
{
	char	*status;
	int		failed;

	status = ft_itoa(shell->last_status);
	if (!status)
		return (1);
	failed = ft_hd_append(result, status);
	free(status);
	*i += 2;
	return (failed);
}

static int	ft_hd_dollar(char **result, char *line, int *i, t_shell *shell)
{
	char	*name;
	char	*value;
	int		start;

	if (line[*i + 1] == '?')
		return (ft_hd_status(result, i, shell));
	if (ft_isdigit(line[*i + 1]))
	{
		*i += 2;
		return (0);
	}
	if (!ft_isalpha(line[*i + 1]) && line[*i + 1] != '_')
		return (ft_hd_literal(result, '$', i));
	start = *i + 1;
	*i = start;
	while (ft_isalnum(line[*i]) || line[*i] == '_')
		(*i)++;
	name = ft_substr(line, start, *i - start);
	if (!name)
		return (1);
	value = get_env_val(shell, name);
	free(name);
	if (!value)
		value = "";
	return (ft_hd_append(result, value));
}

char	*ft_expand_heredoc(char *line, t_shell *shell)
{
	char	*result;
	int		i;
	int		failed;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			failed = ft_hd_dollar(&result, line, &i, shell);
		else
			failed = ft_hd_literal(&result, line[i], &i);
		if (failed)
		{
			free(result);
			return (NULL);
		}
	}
	return (result);
}
