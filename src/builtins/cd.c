/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:01:27 by grhaddad          #+#    #+#             */
/*   Updated: 2026/08/26 20:50:24 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	cd_error(t_shell *shell, const char *msg, const char *arg)
{
	if (arg)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd((char *)arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd((char *)msg, STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putendl_fd((char *)msg, STDERR_FILENO);
	}
	shell->last_status = 1;
	return (1);
}

static int	cd_perror(t_shell *shell, const char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	perror(path);
	shell->last_status = 1;
	return (1);
}

static int	ft_resolve_cd_target(t_shell *shell, t_cmd *cmd, char **target)
{
	char	*value;

	*target = cmd->args[1];
	if (!*target || (*target && (*target)[0] == '~' && (*target)[1] == '\0'))
	{
		*target = get_env_val(shell, "HOME");
		if (!*target || !**target)
			return (cd_error(shell, "HOME not set", NULL));
	}
	else if (cmd->args[2])
		return (cd_error(shell, "too many arguments", NULL));
	if (*target && (*target)[0] == '-' && (*target)[1] == '\0')
	{
		value = get_env_val(shell, "OLDPWD");
		if (!value || !*value)
			return (cd_error(shell, "OLDPWD not set", NULL));
		*target = value;
		ft_putendl_fd(*target, STDOUT_FILENO);
	}
	return (0);
}

int	cd_cmd(t_shell *shell, t_cmd *cmd)
{
	char	cwd[PATH_MAX];
	char	*old_pwd;
	char	*target;

	if (ft_resolve_cd_target(shell, cmd, &target))
		return (shell->last_status);
	old_pwd = get_env_val(shell, "PWD");
	if (!old_pwd && getcwd(cwd, sizeof(cwd)))
		old_pwd = cwd;
	if (chdir(target) != 0)
		return (cd_perror(shell, target));
	if (old_pwd)
		set_env_val(shell, "OLDPWD", old_pwd);
	if (getcwd(cwd, sizeof(cwd)))
		set_env_val(shell, "PWD", cwd);
	shell->last_status = 0;
	return (0);
}
