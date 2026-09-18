/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 23:59:01 by grhaddad          #+#    #+#             */
/*   Updated: 2026/09/16 11:36:20 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_error_msg(const char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	if (msg)
		ft_putendl_fd((char *)msg, 2);
	else
		ft_putendl_fd("error", 2);
	return (1);
}

int	ft_syntax_error(const char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token && *token)
		ft_putstr_fd((char *)token, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putendl_fd("'", 2);
	return (2);
}

int	ft_unclosed_quote_error(void)
{
	ft_putendl_fd("minishell: syntax error: unclosed quote", 2);
	return (2);
}

int	ft_print_error(char *name, char *msg, int code)
{
	char	*line;

	line = ft_strjoin("minishell: ", name);
	line = ft_strjoin_free(line, ": ");
	line = ft_strjoin_free(line, msg);
	line = ft_strjoin_free(line, "\n");
	if (!line)
		return (code);
	write(STDERR_FILENO, line, ft_strlen(line));
	free(line);
	return (code);
}

int	ft_print_errno(char *name, int code)
{
	char	*prefix;
	int		saved_errno;

	saved_errno = errno;
	prefix = ft_strjoin("minishell: ", name);
	errno = saved_errno;
	if (!prefix)
		perror("minishell");
	else
		perror(prefix);
	free(prefix);
	return (code);
}
