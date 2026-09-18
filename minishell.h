/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grhaddad <grhaddad@student.42beirut.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 00:05:16 by grhaddad          #+#    #+#             */
/*   Updated: 2026/09/18 19:50:34 by ysarrouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <limits.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

extern int	g_signal;

/* TOKEN TYPES */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_EOF,
}	t_token_type;

/* BUILTIN TYPES */
typedef enum e_builtin_type
{
	BUILTIN_NONE,
	BUILTIN_ECHO,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXPORT,
	BUILTIN_UNSET,
	BUILTIN_ENV,
	BUILTIN_EXIT
}	t_builtin_type;

/* TOKEN */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/* REDIRECTION */
typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	char			*delimiter;
	struct s_redir	*next;
	int				heredoc_expand;
	int				fd;
}	t_redir;

/* COMMAND */
typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

/* SHELL STATE */
typedef struct s_shell
{
	t_cmd	*cmds;
	char	**env;
	char	*input;
	int		last_status;
}	t_shell;

/* EXPAND */
typedef struct s_expand
{
	char	*str;
	int		*i;
	char	quote;
	t_shell	*shell;
}	t_expand;

/* PIPE STATE */
typedef struct s_pipe_state
{
	int		*prev_fd;
	pid_t	*last_pid;
	int		*failed;
}	t_pipe_state;

/* LEXER */
t_token			*ft_tokenize(char *input);
t_token			*ft_new_token(t_token_type type, char *value);
void			ft_token_add_back(t_token **head, t_token *current);
void			ft_free_tokens(t_token *head);
int				ft_is_stop_char(char c);
t_token			*ft_get_operator_token(char *input, int *i);
t_token			*ft_get_quoted_token(char *input, int *i);
t_token			*ft_get_word_token(char *input, int *i);
int				ft_skip_quoted_sequence(char *input, int *i);

/* PARSER */
t_cmd			*ft_parse_cmd(t_token **tokens);
int				ft_parse_redir(t_cmd *cmd, t_token **tokens);
int				ft_is_redir(t_token_type type);
int				ft_args_add_back(t_cmd *cmd, char *value);
void			ft_cmd_add_back(t_cmd **head, t_cmd *cmd);
void			ft_free_args(char **args);
void			ft_free_redirs(t_redir *redir);
void			ft_free_cmds(t_cmd *head);
t_cmd			*ft_parse(t_token *tokens);
void			ft_redir_add_back(t_cmd *cmd, t_redir *new_node);

/* EXPAND */
void			ft_expand(t_cmd *cmds, t_shell *shell);
char			*ft_expand_str(char *str, t_shell *shell);
char			*ft_get_var_value(char *name, t_shell *shell);
char			*ft_get_var_name(char *str, int *i);
char			*ft_strjoin_free(char *s1, char *s2);
char			*ft_char_to_str(char c);
int				ft_process_backslash(t_expand exp, char **result);

/* ERROR */
int				ft_error_msg(const char *msg);
int				ft_syntax_error(const char *token);
int				ft_unclosed_quote_error(void);
int				ft_validate_quotes(char *input);
int				ft_validate_syntax(t_token *tokens);
int				ft_print_error(char *name, char *msg, int code);
int				ft_print_errno(char *name, int code);

/* BUILTIN */
int				pwd_cmd(t_shell *shell, t_cmd *cmd);
int				echo_cmd(t_shell *shell, t_cmd *cmd);
int				cd_cmd(t_shell *shell, t_cmd *cmd);
int				set_env_val(t_shell *shell, const char *key, const char *value);
char			*get_env_val(t_shell *shell, const char *key);
int				env_cmd(t_shell *shell, t_cmd *cmd);
int				add_new_env(t_shell *shell, char *new_entry);
int				export_cmd(t_shell *shell, t_cmd *cmd);
int				print_sorted_export(t_shell *shell);
int				unset_cmd(t_shell *shell, t_cmd *cmd);
int				exit_cmd(t_shell *shell, t_cmd *cmd);
void			init_environment(t_shell *shell, char **envp);
void			free_environment(t_shell *shell);
char			*ft_get_cmd_path(char *cmd, t_shell *shell);
t_builtin_type	ft_builtin_type(char *cmd);
int				ft_builtin_in_parent(t_builtin_type type);
int				ft_execute_builtin(t_shell *shell, t_cmd *cmd,
					t_builtin_type type);

/* EXECUTION */
void			ft_execute(t_shell *shell);
int				ft_execute_parent_builtin(t_shell *shell, t_cmd *cmd,
					t_builtin_type type);
int				ft_execute_pipeline(t_shell *shell);
int				ft_execute_child_cmd(t_shell *shell, t_cmd *cmd);
int				ft_execute_external(t_shell *shell, t_cmd *cmd);
int				ft_redirection(t_redir *redirs);
int				ft_save_stdio(int *stdin_fd, int *stdout_fd);
void			ft_restore_stdio(int stdin_fd, int stdout_fd);
int				ft_execute_redir_only(t_cmd *cmd);
void			ft_child_exit(t_shell *shell, char *path, int code);

/* SIGNALS */
void			setup_signals(void);
void			ft_signals_exec(void);
void			ft_signals_child(void);

/* HEREDOC */
int				ft_prepare_heredocs(t_shell *shell);
void			ft_cleanup_heredocs(t_cmd *cmds);
int				ft_run_heredocs(t_shell *shell);
int				ft_collect_heredocs(t_shell *shell);
int				ft_hd_write_all(int fd, char *str);
char			*ft_expand_heredoc(char *line, t_shell *shell);
char			*ft_hd_delimiter(char *raw, int *expand);
char			*ft_hd_temp_path(int *counter);
void			ft_discard_heredocs(t_cmd *cmds);

/* PIPELINE */
void			ft_close_fd(int *fd);
int				ft_wait_all(pid_t last_pid, int count);

/* CLEANUP */
void			ft_free_shell(t_shell *shell);
void			ft_shell_exit(t_shell *shell, int code);

/* MAIN */
int				ft_build_cmd(char *input, t_shell *shell);
int				ft_is_blank(const char *input);
int				ft_has_unclosed_quote(char *input);
char			*ft_read_input(void);
int				ft_process_input(char *input, t_shell *shell);

#endif
