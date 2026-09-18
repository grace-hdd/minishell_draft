NAME		:= minishell

CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror
DEPFLAGS	:= -MMD -MP

SRC_DIR		:= src
OBJ_DIR		:= obj
INC_DIR		:= includes
LIBFT_DIR	:= libft
LIBFT		:= $(LIBFT_DIR)/libft.a

# ---------------------------------------------------------------- platform --

UNAME_S		:= $(shell uname -s)

ifeq ($(UNAME_S), Darwin)
RL_DIR		:= $(shell brew --prefix readline)
RL_INC		:= -I$(RL_DIR)/include
RL_LIB		:= -L$(RL_DIR)/lib -lreadline
else
RL_INC		:=
RL_LIB		:= -lreadline
endif

INCLUDES	:= -I$(INC_DIR) -I$(SRC_DIR) -I$(LIBFT_DIR) $(RL_INC)
LDFLAGS		:= $(RL_LIB)

# ----------------------------------------------------------------- sources --

SRC_MAIN	:= main.c \
			   cleanup.c \
			   signals.c \
			   error.c \
			   input_utils.c

SRC_LEXER	:= tokenize.c \
			   tokenize_utils.c \
			   token_create.c \
			   token_list.c \
			   token_type.c \
			   handle_quotes.c

SRC_PARSER	:= parse.c \
			   parse_cmd.c \
			   parse_redir.c \
			   parse_redir_utils.c \
			   parse_utils.c \
			   syntax_validate.c

SRC_EXPAND	:= expand.c \
			   expand_utils.c \
			   expand_var.c

SRC_ENV		:= env_init.c \
			   env_utils.c

SRC_BUILTIN	:= builtin_dispatch.c \
			   cd.c \
			   echo.c \
			   env.c \
			   export.c \
			   export_utils.c \
			   exit.c \
			   pwd.c \
			   unset.c

SRC_HEREDOC	:= heredoc_collect.c \
			   heredoc_prepare.c \
			   heredoc_expand.c \
			   heredoc_process.c \
			   heredoc_utils.c

SRC_EXEC	:= execute.c \
			   execute_utils.c \
			   external.c \
			   cmd_path.c \
			   pipe.c \
			   pipe_utils.c \
			   redirections.c

SRCS		:= $(addprefix main/,     $(SRC_MAIN)) \
			   $(addprefix lexer/,    $(SRC_LEXER)) \
			   $(addprefix parser/,   $(SRC_PARSER)) \
			   $(addprefix expander/, $(SRC_EXPAND)) \
			   $(addprefix env/,      $(SRC_ENV)) \
			   $(addprefix builtins/, $(SRC_BUILTIN)) \
			   $(addprefix heredoc/,  $(SRC_HEREDOC)) \
			   $(addprefix executor/, $(SRC_EXEC))

OBJS		:= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
DEPS		:= $(OBJS:.o=.d)

# ------------------------------------------------------------------ colors --

GREEN		:= \033[0;32m
BLUE		:= \033[0;34m
RESET		:= \033[0m

# ------------------------------------------------------------------ rules ---

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@printf "$(GREEN)[minishell] built$(RESET)\n"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@printf "$(BLUE)[cc]$(RESET) %s\n" $<
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)
	@printf "$(GREEN)[minishell] objects removed$(RESET)\n"

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME)
	@printf "$(GREEN)[minishell] binary removed$(RESET)\n"

re: fclean all

norm:
	@norminette $(SRC_DIR) $(INC_DIR) $(LIBFT_DIR)

-include $(DEPS)

.PHONY: all clean fclean re norm
