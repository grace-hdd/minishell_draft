/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/30 14:00:40 by grhaddad          #+#    #+#             */
/*   Updated: 2026/07/30 14:00:40 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_env_copy(char **env, int filled)
{
	while (filled > 0)
	{
		filled--;
		free(env[filled]);
	}
	free(env);
}

static char	**copy_env(char **envp)
{
	char	**copy;
	int		count;
	int		i;

	count = 0;
	while (envp && envp[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
			return (free_env_copy(copy, i), NULL);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

static void	init_default_env(t_shell *shell)
{
	char	cwd[PATH_MAX];
	char	*shlvl;
	int		level;

	shlvl = get_env_val(shell, "SHLVL");
	if (!shlvl)
		level = 1;
	else
		level = ft_atoi(shlvl) + 1;
	if (level < 0)
		level = 0;
	shlvl = ft_itoa(level);
	if (shlvl)
	{
		set_env_val(shell, "SHLVL", shlvl);
		free(shlvl);
	}
	if (getcwd(cwd, sizeof(cwd)))
		set_env_val(shell, "PWD", cwd);
}

void	init_environment(t_shell *shell, char **envp)
{
	shell->env = copy_env(envp);
	if (!shell->env)
		return ;
	init_default_env(shell);
}

void	free_environment(t_shell *shell)
{
	int	i;

	if (!shell || !shell->env)
		return ;
	i = 0;
	while (shell->env[i])
	{
		free(shell->env[i]);
		i++;
	}
	free(shell->env);
	shell->env = NULL;
}
