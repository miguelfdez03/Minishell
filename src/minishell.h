#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

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
	SIMPLE_Q,
	ARGS,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				space;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				index;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	char			**path;
	t_env			*env;
	char			*input;
	struct s_cmd	*cmd;
	t_token			*tokens;
	int				pipe_flag;
	int				exit_status;
}	t_data;

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

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			*name;
	char			**args;
	t_builtin_type	builtin_id;
	t_redir			*redirections;
	struct s_cmd	*next;
}	t_cmd;

//--UTILS--
int				is_space(char c);
int				is_quotes(char c);
int				is_symbols(char c);
int				ft_strcmp(char *str1, char *str2, int i);
int				ft_strcmp2(char *str1, char *str2);
void			add_to_token(t_token **tokens, t_token_type type, char *value);
void			set_token_space(t_token *tokens, int has_space);
void			free_tokens(t_token *tokens);
int				ft_word_length(char *line, int i);

//--PIPES--
int				handle_pipe(t_cmd *current_cmd, t_token **tokens);
int				process_pipes(t_data *data);
int				execute_pipeline(t_data *data);
int				execute_single_cmd(t_data *data, t_cmd *cmd, int input_fd,
					int output_fd);
void			setup_child_fds(int input_fd, int output_fd);
void			exec_cmd_in_child(t_data *data, t_cmd *cmd, int is_last_cmd);
int				handle_pipe_cmd(t_data *data, t_cmd *cmd, int *prev_fd);
void			process_cmd_args(t_cmd *cmd, t_token **tokens);
int				init_next_cmd(t_cmd *current_cmd, t_token **tokens);
int				init_next_cmd_name(t_cmd *next_cmd, t_token *tmp);
void			close_all_fds(void);
void			cleanup_and_exit(t_data *data, t_cmd *original_cmd, int code);
void			exec_external_cmd(t_data *data, t_cmd *cmd, t_cmd *original);
//--CONCATENATE--
void			concatenate_tokens(t_token **tokens);

//--EXPAND VARS--
int				is_valid_var_char(char c, int is_first);
char			*get_var_value(char *var_name, t_env *env);
char			*extract_var_name(char *str, int start, int *len);
char			*append_to_result(char *result, char *to_add);
char			*append_char(char *result, char c);
char			*expand_exit_status(char *result, int exit_status, int *i);
char			*expand_pid(char *result, int *i);
char			*expand_braced_var(char *str, int *i, t_env *env, char *result);
char			*expand_simple_var(char *str, int *i, t_env *env, char *result);
char			*handle_dollar(char *str, int *i, t_env *env, int exit_status);
char			*expand_string(char *str, t_env *env, int exit_status);
void			expand_variables(t_token *tokens, t_env *env, int exit_status);

//--CMD UTILS--
void			init_cmd(t_cmd *cmd);
t_builtin_type	identify_builtin(char *cmd);
int				create_cmd(char *cmd_name, t_cmd *cmd);
void			add_cmd_arg(t_cmd *cmd, char *arg);
void			free_cmd(t_cmd *cmd);
void			print_cmd_list(t_cmd *cmd);
t_cmd			*tokens_to_cmd(t_token *tokens);
int				init_first_cmd(t_data **data, t_token **tmp);
//--REDIR UTILS--
t_redir			*create_redir(t_token_type type, char *file);
void			add_redir(t_redir **redir, t_token_type type, char *value);
void			free_redirs(t_redir *redir);
int				apply_redirections(t_data *data);
int				handle_heredoc(char *delimiter, t_data *data);
char			*remove_quotes_from_delimiter(char *delimiter);
int				should_expand_heredoc(char *original_delimiter);
void			write_heredoc_interactive(int fd, char *clean_delim, int expand,
					t_data *data);
void			write_heredoc_pipe(int fd, char *clean_delim, int expand,
					t_data *data);

//--BUILT-INS--
int				execute_command(t_data *data);
int				execute_builtin_by_id(t_data *data);
int				builtin_exit(t_data *data);
int				builtin_pwd(t_data *data);
int				builtin_env(t_data *data);
int				builtin_echo(t_data *data);
int				builtin_cd(t_data *data);
int				builtin_export(t_data *data);
int				builtin_unset(t_data *data);

//--CD UTILS--
int				count_args(char **args);
char			*get_cd_path(t_cmd *cmd, t_env *env);
int				do_chdir(char *path, char *oldpwd);
void			update_env_pwd(t_data *data, char *oldpwd, char *newpwd);

//--EXPORT UTILS--
int				is_valid_identifier(char *name);
void			parse_export_arg(char *arg, char **key, char **value);
void			swap_env_nodes(t_env *a, t_env *b);
//--LEXER--
int				lexer(char *line, t_data **data);
void			process_first_cmd_args(t_token **tmp, t_data **data);
int				check_and_exp(t_data **data);
int				check_syntax(t_data **data);
int				handle_quotes(char *line, int i, t_data **data);
int				handle_args(char *line, int i, t_data **data);
int				check_for_closed(char *line, int i, char quote);
char			check_unclosed_quotes(char *line);
int				check_redir(char *line, int i, t_data **data);
int				handle_words(char *line, int i, t_data **data);
int				handle_quotes_and_symbols(char *line, int i, t_data **data,
					int has_space);
int				check_redir_syntax(t_token *current);
int				check_first_token(t_token *current);
int				check_tokens_loop(t_token *current);
int				is_redir_token(t_token_type type);
int				is_operator_token(t_token_type type);
void			print_redir_error(t_token_type type);
int				check_pipe_syntax(t_token *current);

//--MINI_INIT--
int				main_loop(int argc, char **argv, t_data **data);
void			init_tokens(t_token *token);
int				init_cmd_data(t_data **data);
int				init_data(t_data **data, char **env, t_env *env_t);
void			free_data(t_data *data);

//--ENV--
int				init_env(char **env, t_env *env_t);
void			free_env(t_env *env);
t_env			*find_env(t_env *env, const char *key);
char			*get_env_value_from_list(t_env *env, char *key);
void			set_env_value(t_env **env_head, const char *key,
					const char *value);
void			init_first_env_node(t_env **env_head, char *dup_key,
					char *dup_val);
void			add_env_node(t_env *last, char *dup_key, char *dup_val);
void			set_env_new_node(t_env **env_head, char *dup_key,
					char *dup_val);
void			unset_env_var(t_env **env_head, const char *key);
int				execute_external_command(t_data *data);

int				count_env_vars(t_env *env);
char			**env_list_to_array(t_env *env);
char			**build_args_array(t_cmd *cmd);
char			*check_absolute_path(char *cmd);
char			*find_cmd_in_path(char *cmd, t_env *env);
int				handle_fork_error(char *cmd_path, char **args,
					char **env_array);
int				wait_and_cleanup(pid_t pid, char *cmd_path, char **args,
					char **env_array);

char			*find_command_path(char *cmd, char **envp);
char			*search_in_path_dirs(char *path_copy, char *cmd);
char			*get_env_value(char *var_name, char **envp);
char			*build_full_path(char *dir, char *cmd);
void			free_string_array(char **array);

//--SIGNALS--
extern volatile sig_atomic_t	g_signal_received;
void			handle_sigint(int sig);
void			setup_signals_interactive(void);
void			setup_signals_child(void);
void			setup_signals_heredoc(void);
void			setup_signals_executing(void);

#endif