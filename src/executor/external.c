/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 09:22:42 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/09/16 11:28:18 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_cmd_error(char *cmd, char *msg, int code)
{
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
	return (code);
}

static int	ft_check_slash_target(char *cmd)
{
	struct stat	st;

	if (!ft_strchr(cmd, '/'))
		return (0);
	if (stat(cmd, &st) != 0)
	{
		if (errno == EACCES)
			return (ft_cmd_error(cmd, "Permission denied", 126));
		return (ft_print_error(cmd, "No such file or directory", 127));
	}
	if (S_ISDIR(st.st_mode))
		return (ft_print_error(cmd, "Is a directory", 126));
	if (access(cmd, X_OK) != 0)
		return (ft_print_error(cmd, "Permission denied", 126));
	return (0);
}

static int	ft_exec_fail_code(void)
{
	if (errno == ENOENT)
		return (127);
	return (126);
}

int	ft_execute_external(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	int		code;

	path = NULL;
	code = ft_check_slash_target(cmd->args[0]);
	if (code != 0)
		ft_child_exit(shell, NULL, code);
	path = ft_get_cmd_path(cmd->args[0], shell);
	if (!path)
		ft_child_exit(shell, NULL,
			ft_print_error(cmd->args[0], "command not found", 127));
	execve(path, cmd->args, shell->env);
	code = ft_exec_fail_code();
	ft_print_errno(cmd->args[0], code);
	ft_child_exit(shell, path, code);
	return (code);
}
