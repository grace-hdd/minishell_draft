/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 16:00:02 by grhaddad          #+#    #+#             */
/*   Updated: 2026/08/26 20:05:37 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_update_quote(char c, char *quote)
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

char	*ft_get_var_name(char *str, int *i)
{
	int	start;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (ft_isdigit(str[*i]))
	{
		(*i)++;
		return (ft_substr(str, start, 1));
	}
	if (!(ft_isalnum(str[*i]) || str[*i] == '_'))
		return (NULL);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 || !s2)
	{
		free(s1);
		return (NULL);
	}
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char	*ft_char_to_str(char c)
{
	char	*str;

	str = malloc(2);
	if (!str)
		return (NULL);
	str[0] = c;
	str[1] = '\0';
	return (str);
}

int	ft_process_backslash(t_expand exp, char **result)
{
	char	*tmp;

	if (exp.str[*exp.i + 1] == '\0')
	{
		tmp = ft_char_to_str('\\');
		(*exp.i)++;
	}
	else if (exp.quote == 0 || (exp.quote == '"'
			&& (exp.str[*exp.i + 1] == '$'
				|| exp.str[*exp.i + 1] == '`'
				|| exp.str[*exp.i + 1] == '"'
				|| exp.str[*exp.i + 1] == '\\')))
	{
		tmp = ft_char_to_str(exp.str[*exp.i + 1]);
		*exp.i += 2;
	}
	else
		tmp = ft_char_to_str(exp.str[(*exp.i)++]);
	if (!tmp)
		return (1);
	*result = ft_strjoin_free(*result, tmp);
	free(tmp);
	return (*result == NULL);
}
