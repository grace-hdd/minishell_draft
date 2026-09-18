/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 12:52:43 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/08/26 20:23:02 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_hd_write_all(int fd, char *str)
{
	ssize_t	written;
	ssize_t	offset;
	ssize_t	length;

	offset = 0;
	length = ft_strlen(str);
	while (offset < length)
	{
		written = write(fd, str + offset, length - offset);
		if (written <= 0)
			return (1);
		offset += written;
	}
	return (0);
}

static void	ft_hd_remove_quotes(char *raw, char *clean, int *expand)
{
	char	quote;
	int		i;
	int		j;

	*expand = 1;
	quote = 0;
	i = 0;
	j = 0;
	while (raw[i])
	{
		if (!quote && (raw[i] == '\'' || raw[i] == '"'))
		{
			quote = raw[i];
			*expand = 0;
		}
		else if (quote && raw[i] == quote)
			quote = 0;
		else
			clean[j++] = raw[i];
		i++;
	}
	clean[j] = '\0';
}

char	*ft_hd_delimiter(char *raw, int *expand)
{
	char	*clean;

	if (!raw)
		return (NULL);
	clean = malloc(ft_strlen(raw) + 1);
	if (!clean)
		return (NULL);
	ft_hd_remove_quotes(raw, clean, expand);
	return (clean);
}

static char	*ft_hd_path(int counter)
{
	char	*number;
	char	*path;

	number = ft_itoa(counter);
	if (!number)
		return (NULL);
	path = ft_strjoin("/tmp/.minishell_hd_", number);
	free(number);
	return (path);
}

char	*ft_hd_temp_path(int *counter)
{
	char	*path;
	int		fd;

	while (*counter < INT_MAX)
	{
		path = ft_hd_path(*counter);
		if (!path)
			return (NULL);
		(*counter)++;
		fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (fd >= 0)
		{
			close(fd);
			return (path);
		}
		free(path);
		if (errno != EEXIST)
			return (NULL);
	}
	return (NULL);
}
