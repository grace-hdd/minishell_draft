*This project has been created as part of the 42 curriculum by grhaddad, ysarrouh.*

# minishell

A simplified Unix shell written in C, recreating the core behaviour of Bash:
command execution, pipes, redirections, here-documents, environment variable
expansion, quote handling, built-in commands and signal handling.

---

## Description

`minishell` reads a line from an interactive prompt, turns it into a list of
commands, and runs them the way Bash would.

The goal of the project is to understand, from the inside, what happens between
the moment a user presses Enter and the moment a program starts: how a line of
text becomes tokens, how tokens become a command structure, how variables are
expanded, how processes are created with `fork()` and replaced with `execve()`,
and how file descriptors are rewired to implement pipes and redirections.

The shell goes through five stages for every line of input:

| Stage | Directory | What it does |
|---|---|---|
| **Lexer** | `src/lexer/` | Splits the raw input into tokens (words, `\|`, `<`, `>`, `<<`, `>>`), keeping quoted sequences intact. |
| **Parser** | `src/parser/` | Validates the syntax and builds a linked list of commands (`t_cmd`), each carrying its arguments and its list of redirections (`t_redir`). |
| **Expander** | `src/expander/` | Replaces `$VAR` and `$?` with their values, applying single- and double-quote rules. |
| **Heredoc** | `src/heredoc/` | Collects `<<` input into temporary files before execution, expanding variables unless the delimiter is quoted. |
| **Executor** | `src/executor/` | Applies redirections, creates the pipeline, forks children, runs built-ins or external binaries, and waits for the exit status. |

The shell state lives in a single `t_shell` structure (environment copy, current
command list, last exit status). As required by the subject, exactly **one global
variable** is used, `g_signal`, and it only ever stores a signal number.

---

## Features

### Command execution
- Executes binaries found through `PATH`, or via a relative/absolute path.
- Returns the correct exit status, including `127` (command not found) and
  `126` (permission denied / is a directory).

### Pipes
- Any number of commands chained with `|`; each command's output is connected to
  the next command's input.
- The pipeline's exit status is the exit status of its last command.

### Redirections
| Operator | Effect |
|---|---|
| `<` | Redirect input from a file |
| `>` | Redirect output to a file (truncate) |
| `>>` | Redirect output to a file (append) |
| `<<` | Here-document: read input until a line matching the delimiter |

Multiple redirections on the same command are applied left to right.

### Quotes
- `'single quotes'` prevent every interpretation, including `$`.
- `"double quotes"` prevent every interpretation **except** `$`.

### Expansion
- `$VAR` expands to its value in the shell environment, or to an empty string
  when unset.
- `$?` expands to the exit status of the last foreground pipeline.
- Inside a here-document, expansion happens unless the delimiter itself was
  quoted (`<< "EOF"`).

### Built-in commands
| Built-in | Supported form |
|---|---|
| `echo` | with the `-n` option |
| `cd` | with a relative or absolute path |
| `pwd` | no options |
| `export` | no options; with no argument, prints the sorted environment |
| `unset` | no options |
| `env` | no options, no arguments |
| `exit` | with an optional numeric status |

Built-ins that modify the shell state (`cd`, `export`, `unset`, `exit`) run in
the parent process when they are alone on the line; inside a pipeline they run
in the child, like Bash.

### Signals (interactive mode)
| Key | Behaviour |
|---|---|
| `Ctrl-C` | Prints a newline and displays a fresh prompt; sets the exit status to `130` |
| `Ctrl-D` | Exits the shell |
| `Ctrl-\` | Does nothing |

While a command is running, the shell ignores `SIGINT`/`SIGQUIT` and the child
restores the default handlers, so `Ctrl-C` kills the running command and not the
shell itself.

### History
Line editing and history are provided by GNU Readline; non-empty lines are added
to the history with `add_history()`, and the history is freed on exit with
`rl_clear_history()`.

---

## Instructions

### Clone and build

```bash
git clone <repository-url> minishell
cd minishell
make
```

`make` first builds `libft/libft.a`, then compiles every source file into
`obj/` and links the `minishell` executable.

### Makefile rules

| Rule | Effect |
|---|---|
| `make` / `make all` | Build `minishell` |
| `make clean` | Remove the object files (`obj/`) and clean `libft` |
| `make fclean` | `clean` + remove the `minishell` binary and `libft.a` |
| `make re` | `fclean` + rebuild everything |
| `make norm` | Run `norminette` on `src/`, `includes/` and `libft/` |

Compilation flags: `-Wall -Wextra -Werror`. Dependency files (`-MMD -MP`) are
used so that editing a header only recompiles the files that include it, and no
unnecessary relinking is performed.

### Run

```bash
./minishell
```

The prompt `minishell$ ` appears and accepts commands like a regular shell.
Type `exit` or press `Ctrl-D` to leave.

---

## Usage examples

```bash
minishell$ echo -n "hello" ; echo " world"      # ';' is not interpreted
minishell$ ls -la | grep ".c" | wc -l
12

minishell$ cat < Makefile > copy.txt
minishell$ echo "appended line" >> copy.txt

minishell$ cat << EOF
> $USER is here
> EOF
ysarrouh is here

minishell$ cat << "EOF"
> $USER stays literal
> EOF
$USER stays literal

minishell$ export NAME=42
minishell$ echo "$NAME" '$NAME'
42 $NAME

minishell$ ls /nonexistent
ls: /nonexistent: No such file or directory
minishell$ echo $?
1

minishell$ cd /tmp && pwd    # '&&' is part of the bonus, not implemented
minishell$ exit
exit
```

---

## Project structure

```
minishell/
├── minishell.h                 # Shared types and prototypes
├── Makefile
├── libft/                      # Personal C library (built first)
├── includes/
└── src/
    ├── main/
    │   ├── main.c              # Entry point, readline loop, history
    │   ├── input_utils.c       # Line -> tokens -> parse -> expand -> execute
    │   ├── signals.c           # SIGINT / SIGQUIT handlers (interactive, exec, child)
    │   ├── error.c             # Error messages and exit codes
    │   └── cleanup.c           # Freeing the shell state
    ├── lexer/
    │   ├── tokenize.c          # Main tokenizer loop
    │   ├── tokenize_utils.c
    │   ├── token_create.c      # t_token allocation
    │   ├── token_list.c        # Linked-list helpers
    │   ├── token_type.c        # Operator recognition
    │   └── handle_quotes.c     # Skipping quoted sequences
    ├── parser/
    │   ├── parse.c             # Tokens -> command list
    │   ├── parse_cmd.c         # One command: arguments + redirections
    │   ├── parse_redir.c       # Redirection nodes
    │   ├── parse_redir_utils.c
    │   ├── parse_utils.c
    │   └── syntax_validate.c   # Syntax errors, unclosed quotes
    ├── expander/
    │   ├── expand.c            # Walks the command list
    │   ├── expand_var.c        # $VAR and $? lookup
    │   └── expand_utils.c      # Quote state, string building
    ├── env/
    │   ├── env_init.c          # Copy of envp at startup
    │   ├── env_find.c          # Key lookup
    │   └── env_utils.c         # Get / set / add entries
    ├── builtins/
    │   ├── builtin_dispatch.c  # Built-in identification and dispatch
    │   ├── echo.c  cd.c  pwd.c
    │   ├── export.c  export_utils.c
    │   ├── unset.c  env.c  exit.c
    ├── heredoc/
    │   ├── heredoc_prepare.c   # Pre-execution pass over all here-docs
    │   ├── heredoc_collect.c   # Reading lines until the delimiter
    │   ├── heredoc_expand.c    # Expansion inside the body
    │   ├── heredoc_process.c
    │   └── heredoc_utils.c     # Temporary file paths, cleanup
    └── executor/
        ├── execute.c           # Entry point of execution
        ├── execute_utils.c
        ├── pipe.c              # Pipeline creation
        ├── pipe_utils.c        # fd closing, waiting for children
        ├── redirections.c      # dup2 of the redirection list
        ├── external.c          # fork + execve
        └── cmd_path.c          # PATH resolution
```

---

## Technical choices

- **Linked lists over arrays.** Tokens, commands and redirections are all singly
  linked lists, so the number of pipes or redirections is never bounded and
  freeing is uniform (`ft_free_tokens`, `ft_free_cmds`, `ft_free_redirs`).
- **Here-docs are collected before execution.** Every `<<` in the line is read
  into a temporary file during a first pass, so a pipeline like
  `cat << EOF | wc -l` reads its input before any process is forked.
- **One environment copy.** `envp` is duplicated at startup into `shell->env`,
  so `export` and `unset` never touch the process environment directly.
- **A single global.** `g_signal` only stores a signal number, as required; the
  handler never touches the shell's data structures.
- **Norm compliance.** Every function is under 25 lines, no `for` loops are used,
  and the code passes `norminette` (`make norm`).

---

## Resources

Documentation and references used while building the project:

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — the behavioural reference for every ambiguous case.
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) — grammar, quoting rules and expansion order.
- [GNU Readline Library manual](https://tiswww.case.edu/php/chet/readline/readline.html) — `readline`, `add_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`.
- Linux man pages: `fork(2)`, `execve(2)`, `pipe(2)`, `dup2(2)`, `wait(2)`, `waitpid(2)`, `sigaction(2)`, `access(2)`, `open(2)`, `stat(2)`, `chdir(2)`, `getcwd(3)` — [man7.org](https://man7.org/linux/man-pages/).
- *Advanced Programming in the UNIX Environment*, W. Richard Stevens — chapters on process control, process relationships and signals.
- [42 Norm (norminette)](https://github.com/42School/norminette) — coding standard enforced on the whole project.

### Use of AI

- **Understanding concepts.** Asking for explanations of `dup2()` semantics,
  the order of file-descriptor closing in a pipeline, and the difference between
  `signal()` and `sigaction()`. The explanations were then verified against the
  man pages listed above.
- **Debugging.** Describing observed behaviour (a hanging pipeline, a leaked
  file descriptor, a wrong exit status) and using the suggestions as hypotheses
  to test with `valgrind`, `lsof` and comparison runs against Bash.
- **Reviewing the Norm.** Checking which functions exceeded 25 lines and
  discussing how to split them into helper functions, before applying the split
  ourselves.
- **Documentation.** Drafting the structure of this `README.md`, which was then
  corrected and completed by hand against the real source tree.

---
