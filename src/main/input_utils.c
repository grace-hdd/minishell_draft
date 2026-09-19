/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysarrouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 12:07:27 by ysarrouh          #+#    #+#             */
/*   Updated: 2026/09/19 12:07:27 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_build_cmd(char *input, t_shell *shell)
{
	t_token	*tokens;

	if (ft_validate_quotes(input))
		return (2);
	tokens = ft_tokenize(input);
	if (!tokens)
		return (2);
	if (ft_validate_syntax(tokens))
	{
		ft_free_tokens(tokens);
		return (2);
	}
	shell->cmds = ft_parse(tokens);
	ft_free_tokens(tokens);
	if (!shell->cmds)
		return (2);
	return (0);
}

static int	ft_is_blank(const char *input)
{
	if (!input)
		return (1);
	while (*input == ' ' || *input == '\t')
		input++;
	return (*input == '\0');
}

static int	ft_has_unclosed_quote(char *input)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (input && input[i])
	{
		ft_update_quote(input[i], &quote);
		i++;
	}
	return (quote != 0);
}

char	*ft_read_input(void)
{
	char	*input;
	char	*line;
	char	*joined;

	input = readline("minishell$ ");
	while (input && ft_has_unclosed_quote(input))
	{
		line = readline("> ");
		if (!line)
			break ;
		joined = ft_strjoin(input, "\n");
		free(input);
		if (!joined)
		{
			free(line);
			return (NULL);
		}
		input = ft_strjoin(joined, line);
		free(joined);
		free(line);
		if (!input)
			return (NULL);
	}
	return (input);
}

int	ft_process_input(char *input, t_shell *shell)
{
	if (ft_is_blank(input))
		return (shell->last_status);
	if (ft_build_cmd(input, shell))
		return (2);
	if (ft_prepare_heredocs(shell))
	{
		ft_cleanup_heredocs(shell->cmds);
		return (shell->last_status);
	}
	ft_expand(shell->cmds, shell);
	ft_execute(shell);
	ft_cleanup_heredocs(shell->cmds);
	return (shell->last_status);
}
