# minishell

A simplified Unix shell written in C that replicates core Bash features such as command execution, pipes, redirections, environment variable expansion, built-in commands, and signal handling to demonstrate process management and system-level programming.

## Features

- **Command Execution**: Execute system commands with arguments
- **Pipes**: Chain multiple commands using `|` operator
- **Redirections**: 
  - Input redirection (`<`)
  - Output redirection (`>`)
  - Append redirection (`>>`)
- **Environment Variable Expansion**: Expand variables like `$VAR` and `$?`
- **Built-in Commands**: Implement shell built-ins such as `cd`, `echo`, `export`, `unset`, `exit`, etc.
- **Signal Handling**: Properly handle signals (Ctrl+C, Ctrl+D, Ctrl+\)
- **Quote Handling**: Support for single and double quotes

## Project Structure

```
minishell/
├── src/
│   ├── main.c                 # Entry point and main shell loop
│   ├── lexer/                 # Tokenization and lexical analysis
│   │   ├── tokenize.c
│   │   ├── token_type.c
│   │   ├── token_create.c
│   │   ├── token_list.c
│   │   ├── handle_quotes.c
│   │   └── tokenize_utils.c
│   ├── parser/                # Parse tokens into AST
│   │   ├── parse.c
│   │   ├── parse_cmd.c
│   │   ├── parse_redir.c
│   │   └── parse_utils.c
│   ├── expand/                # Variable and environment expansion
│   │   ├── expand.c
│   │   ├── expand_var.c
│   │   └── expand_utils.c
│   └── error/                 # Error handling
│       └── error.c
├── includes/
│   └── libft/                 # Custom C library utilities
├── Makefile                   # Build configuration
└── README.md

```

## Requirements

- **OS**: Linux, macOS, or other Unix-like systems
- **Compiler**: GCC or Clang with C99 support
- **Tools**: Make, standard C library (libc)
- **Dependencies**: GNU Readline library (for line editing)

## Installation

### Clone the Repository

```bash
git clone https://github.com/grace-hdd/minishell.git
cd minishell
```

### Build

```bash
make
```

This will compile the project and generate the `minishell` executable.

### Clean Build

```bash
make re
```

## Usage

### Running the Shell

```bash
./minishell
```

The shell will display a prompt and accept commands just like a standard Unix shell.

### Basic Examples

```bash
# Execute a command
$ echo Hello, World!
Hello, World!

# Change directory
$ cd /tmp
$ pwd
/tmp

# Use pipes to chain commands
$ ls | grep .c | wc -l
5

# Redirect output to a file
$ echo "test" > output.txt

# Redirect input from a file
$ cat < input.txt

# Use environment variables
$ echo $HOME
/home/user

# Check exit status of last command
$ echo $?
0

# Exit the shell
$ exit
```

## Implementation Details

### Lexer (Tokenization)
Breaks input into tokens, identifying operators, commands, arguments, redirections, and pipes. Handles quote processing for string literals.

### Parser
Converts tokens into an Abstract Syntax Tree (AST) representing the command structure, including:
- Command chains
- Pipe operators
- Redirections

### Expander
Replaces environment variables and special variables (`$?` for exit status) with their values.

### Executor
- Forks child processes to execute commands
- Manages file descriptors for pipes and redirections
- Handles signal interruption (SIGINT, SIGTERM)

## Building and Compilation

The project uses a Makefile with the following targets:

```bash
make           # Build the executable
make clean     # Remove object files
make fclean    # Remove object files and executable
make re        # Clean and rebuild
```

Compiler flags used:
- `-Wall` : Enable common warnings
- `-Wextra` : Enable extra warnings
- `-Werror` : Treat warnings as errors

## Learning Outcomes

This project demonstrates:
- Process management and forking (`fork()`, `exec()`)
- File descriptor manipulation and redirection
- Signal handling and interrupts
- Parsing and lexical analysis
- Shell scripting internals
- Memory management in C
- Systems programming best practices

## License

This project is created as part of the 42 School curriculum.

## Author

**grhaddad** - [grace-hdd](https://github.com/grace-hdd)

---

For more information about Unix shells and system programming, refer to:
- [The Unix Programming Environment](https://en.wikipedia.org/wiki/Unix_programming)
- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/)
- [Linux man pages](https://man7.org/)
