/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 19:21:50 by grhaddad          #+#    #+#             */
/*   Updated: 2026/08/26 21:41:08 by grhaddad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	g_signal = 0;

static int	ft_shell_loop(t_shell *shell)
{
	while (1)
	{
		setup_signals();
		shell->input = readline("minishell$ ");
		if (g_signal == SIGINT)
		{
			shell->last_status = 130;
			g_signal = 0;
		}
		if (!shell->input)
			break ;
		if (*shell->input)
			add_history(shell->input);
		shell->last_status = ft_process_input(shell->input, shell);
		ft_free_cmds(shell->cmds);
		shell->cmds = NULL;
		free(shell->input);
		shell->input = NULL;
	}
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	return (shell->last_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_environment(&shell, envp);
	if (!shell.env)
		return (ft_error_msg("allocation failure"));
	shell.last_status = 0;
	shell.cmds = NULL;
	shell.input = NULL;
	setup_signals();
	shell.last_status = ft_shell_loop(&shell);
	ft_free_shell(&shell);
	rl_clear_history();
	return (shell.last_status);
}
