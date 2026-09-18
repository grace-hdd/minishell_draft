/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 19:04:07 by grhaddad          #+#    #+#             */
/*   Updated: 2026/09/16 09:31:16 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_numeric_str(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	parse_exit_status(const char *str, int *status)
{
	unsigned long long	value;
	unsigned long long	limit;
	int					negative;
	int					i;

	negative = (str[0] == '-');
	i = (str[0] == '+' || negative);
	value = 0;
	limit = (unsigned long long)LLONG_MAX + (negative != 0);
	while (str[i])
	{
		if (value > (limit - (str[i] - '0')) / 10)
			return (0);
		value = value * 10 + (str[i] - '0');
		i++;
	}
	*status = (int)(value % 256);
	if (negative)
		*status = -*status;
	return (1);
}

static void	numeric_error_exit(t_shell *shell, const char *arg)
{
	write(STDERR_FILENO, "minishell: exit: ", 17);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, ": numeric argument required\n", 28);
	ft_shell_exit(shell, 2);
}

int	exit_cmd(t_shell *shell, t_cmd *cmd)
{
	int		status;
	char	*trimmed;

	if (isatty(STDIN_FILENO) && shell->cmds && !shell->cmds->next)
		write(STDERR_FILENO, "exit\n", 5);
	if (!cmd->args[1])
		ft_shell_exit(shell, shell->last_status);
	trimmed = ft_strtrim(cmd->args[1], " \t\n\v\f\r");
	if (!trimmed || !is_numeric_str(trimmed)
		|| !parse_exit_status(trimmed, &status))
	{
		free(trimmed);
		numeric_error_exit(shell, cmd->args[1]);
	}
	free(trimmed);
	if (cmd->args[2])
	{
		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
		shell->last_status = 1;
		return (1);
	}
	ft_shell_exit(shell, status);
	return (0);
}
