/* ************************************************************************** */
/*                                                                           */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:12:56 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/02 13:20:36 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef enum e_token_type
{
	WORD,
	STRING,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
	EMPTY,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char 			*key;
	char 			*value;
	int				index;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	char			**path;
	t_env			*env;
	char			*input;
	struct s_cmd	*cmd;
	t_token			**t_lexer;
	int				pipe_flag;
}	t_data;

typedef enum e_cmd_type
{
	CMD_EXTERNAL,
	CMD_BUILTIN
}	t_cmd_type;

typedef enum e_builtin_type
{
	BUILTIN_NONE = -1,
	BUILTIN_CD,
	BUILTIN_PWD,
	BUILTIN_EXIT,
	BUILTIN_ECHO,
	BUILTIN_ENV,
	BUILTIN_EXPORT,
	BUILTIN_UNSET
}	t_builtin_type;

typedef struct s_cmd
{
	char			*name;
	char			**args;
	t_cmd_type		type;
	t_builtin_type	builtin_id;
	char			*input_file;
	char			*output_file;
	int				append_mode;
	int				heredoc;
	struct s_cmd	*next;
}	t_cmd;

//--UTILS--
int				is_space(char c);
int				is_quotes(char c);
int				is_symbols(char c);
int				ft_strcmp(char *str1, char *str2, int i);
int				ft_strcmp2(char *str1, char *str2);
void			add_to_token(t_token **tokens, t_token_type type, char *value);
int				ft_word_length(char *line, int i);

//--CMD UTILS--
t_builtin_type	identify_builtin(char *cmd);
t_cmd			*create_cmd(char *cmd_name);
void			add_cmd_arg(t_cmd *cmd, char *arg);
void			free_cmd(t_cmd *cmd);
t_cmd			*parse_simple_input(char *input);

//--BUILT-INS--
int				execute_command(t_cmd *cmd, t_data *data);
int				execute_builtin_by_id(t_cmd *cmd, t_data *data);
int				builtin_exit(t_cmd *cmd, t_data *data);
int				builtin_pwd(t_data *data);
int				builtin_env(t_data *data);
int				builtin_echo(t_cmd *cmd, t_data *data);
int				builtin_cd(t_cmd *cmd, t_data *data);
//--LEXER--
int 			lexer(char *line, t_token **tokens);
int				handle_quotes(char *line, int i, t_token **tokens);
int				check_for_closed(char *line, int i, char quote);
int				check_redir(char *line, int i, t_token **tokens);
int				handle_words(char *line, int i, t_token **tokens);

//--MINI_INIT--
int				main_loop(int argc, char **argv, t_data **data, char **env);
void			init_tokens(t_token *token);
int 			init_data(t_data **data, char **env, t_env *env_t);

//--ENV--
int				init_env(char **env, t_env *env_t);

//--TEST EXECUTOR--
int				test_executor(void);
int				test_simple_command(char *cmd_path, char **args);
int				test_simple_command_with_path(char *cmd, char **args);

//--PATH UTILS--
char			*find_command_path(char *cmd, char **envp);
char			*search_in_path_dirs(char *path_copy, char *cmd);
char			*get_env_value(char *var_name, char **envp);
char			*build_full_path(char *dir, char *cmd);
void			free_string_array(char **array);


#endif