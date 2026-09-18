/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 01:07:03 by grhaddad          #+#    #+#             */
/*   Updated: 2026/07/27 01:07:03 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	env_invalid_option(t_shell *shell, char *arg)
{
	ft_putstr_fd("env: invalid option -- '", STDERR_FILENO);
	if (arg[1])
		ft_putchar_fd(arg[1], STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	ft_putendl_fd("Try 'env --help' for more information.", STDERR_FILENO);
	shell->last_status = 125;
	return (125);
}

static void	env_print_values(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		if (ft_strchr(shell->env[i], '='))
		{
			write(STDOUT_FILENO, shell->env[i], ft_strlen(shell->env[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		i++;
	}
}

int	env_cmd(t_shell *shell, t_cmd *cmd)
{
	if (!shell || !shell->env)
		return (0);
	if (cmd->args[1])
	{
		if (cmd->args[1][0] == '-' && cmd->args[1][1] != '\0')
			return (env_invalid_option(shell, cmd->args[1]));
		ft_putstr_fd("minishell: env: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[1], STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		shell->last_status = 127;
		return (127);
	}
	env_print_values(shell);
	shell->last_status = 0;
	return (0);
}
