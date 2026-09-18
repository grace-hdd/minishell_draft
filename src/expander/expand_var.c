/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 16:00:25 by grhaddad          #+#    #+#             */
/*   Updated: 2026/08/26 21:34:12 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_get_var_value(char *name, t_shell *shell)
{
	int		i;
	size_t	len;

	if (!name || !shell)
		return (ft_strdup(""));
	if (ft_strncmp(name, "?", 2) == 0)
		return (ft_itoa(shell->last_status));
	len = ft_strlen(name);
	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) == 0
			&& shell->env[i][len] == '=')
			return (ft_strdup(shell->env[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

static char	*ft_expand_var(char *str, int *i, t_shell *shell)
{
	char	*name;
	char	*value;

	(*i)++;
	name = ft_get_var_name(str, i);
	if (!name)
		return (NULL);
	value = ft_get_var_value(name, shell);
	free(name);
	if (!value)
		return (ft_strdup(""));
	return (value);
}

static int	ft_update_quote(char c, char *quote)
{
	if (c == '\'' && *quote != '"')
	{
		if (*quote == '\'')
			*quote = 0;
		else
			*quote = '\'';
		return (1);
	}
	if (c == '"' && *quote != '\'')
	{
		if (*quote == '"')
			*quote = 0;
		else
			*quote = '"';
		return (1);
	}
	return (0);
}

static int	ft_process_expand_char(t_expand exp, char **result)
{
	char	*tmp;

	if (exp.str[*exp.i] == '\\' && exp.quote != '\'')
		return (ft_process_backslash(exp, result));
	if (exp.str[*exp.i] == '$' && exp.quote != '\'')
	{
		if (exp.str[*exp.i + 1] == '?'
			|| ft_isalnum(exp.str[*exp.i + 1])
			|| exp.str[*exp.i + 1] == '_')
			tmp = ft_expand_var(exp.str, exp.i, exp.shell);
		else
		{
			(*exp.i)++;
			tmp = ft_char_to_str('$');
		}
	}
	else
		tmp = ft_char_to_str(exp.str[(*exp.i)++]);
	if (!tmp)
		return (1);
	*result = ft_strjoin_free(*result, tmp);
	free(tmp);
	if (!*result)
		return (1);
	return (0);
}

char	*ft_expand_str(char *str, t_shell *shell)
{
	char	*result;
	int		i;
	char	quote;

	if (!str)
		return (ft_strdup(""));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (ft_update_quote(str[i], &quote))
		{
			i++;
			continue ;
		}
		if (ft_process_expand_char((t_expand){str, &i, quote, shell}, &result))
			return (free(result), NULL);
	}
	return (result);
}
