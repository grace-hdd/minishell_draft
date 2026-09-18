/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:01:22 by grhaddad          #+#    #+#             */
/*   Updated: 2026/08/26 20:05:37 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	pwd_print_env(t_shell *shell)
{
	char	*env_pwd;

	env_pwd = ft_get_var_value("PWD", shell);
	if (env_pwd && *env_pwd)
	{
		ft_putendl_fd(env_pwd, STDOUT_FILENO);
		free(env_pwd);
		return (0);
	}
	free(env_pwd);
	return (1);
}

static int	pwd_print_cwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	return (0);
}

int	pwd_cmd(t_shell *shell, t_cmd *cmd)
{
	(void)cmd;
	if (!pwd_print_cwd())
		return (0);
	if (!pwd_print_env(shell))
		return (0);
	ft_putendl_fd("minishell: pwd: error retrieving current directory",
		STDERR_FILENO);
	return (1);
}
