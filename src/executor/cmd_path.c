/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 14:59:00 by grhaddad          #+#    #+#             */
/*   Updated: 2026/07/30 14:59:00 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*ft_build_candidate(char *dir, char *cmd)
{
	char	*tmp;
	char	*path;

	if (!dir || !*dir)
		return (ft_strdup(cmd));
	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (path);
}

static char	*ft_resolve_from_path(char *cmd, char **paths)
{
	char	*candidate;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		candidate = ft_build_candidate(paths[i], cmd);
		if (!candidate)
			return (NULL);
		if (access(candidate, X_OK) == 0)
			return (candidate);
		free(candidate);
		i++;
	}
	return (NULL);
}

char	*ft_get_cmd_path(char *cmd, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	char	*resolved;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_val(shell, "PATH");
	if (!path_env || !*path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	resolved = ft_resolve_from_path(cmd, paths);
	ft_free_args(paths);
	return (resolved);
}
