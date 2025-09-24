/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel <miguel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:12:56 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/24 14:51:40 by miguel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/wait.h>
# include <unistd.h>

// Tipos de tokens que puede reconocer nuestro Spider-Shell
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

typedef struct s_data
{
	char **env;
	char *input;

} t_data;

//--UTILS--
int		is_space(char c);
int		is_quotes(char c);
int		is_symbols(char c);
int		ft_strcmp(char *str1, char *str2, int i);
void	add_to_token(t_token *token, t_token_type type, char *value);
int		ft_word_length(char *line, int i);
//--LEXER--
int 	lexer(char *line);
int		handle_quotes(char *line, int i, t_token *tokens);
int		check_for_closed(char *line, int i, char quote);
int		check_redir(char *line, int i, t_token *tokens);
int		handle_words(char *line, int i, t_token *tokens);

//--MINI_INIT--
int		main_loop(int argc, char **argv, char **env);
void	init_tokens(t_token *token);

//--TEST EXECUTOR--
int					test_executor(void);
int					test_simple_command(char *cmd_path, char **args);
int					test_simple_command_with_path(char *cmd, char **args);

//--PATH UTILS--
char				*find_command_path(char *cmd, char **envp);
char				*search_in_path_dirs(char *path_copy, char *cmd);
char				*get_env_value(char *var_name, char **envp);
char				*build_full_path(char *dir, char *cmd);
void				free_string_array(char **array);

#endif