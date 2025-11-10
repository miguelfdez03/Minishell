#include "../minishell.h"

void	setup_child_fds(int input_fd, int output_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
}

static void	close_all_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
		close(fd++);
}

static void	cleanup_and_exit(t_data *data, t_cmd *original_cmd, int code)
{
	data->cmd = original_cmd;
	rl_clear_history();
	free_data(data);
	exit(code);
}

static void	exec_external_cmd(t_data *data, t_cmd *cmd, t_cmd *original_cmd)
{
	char	*cmd_path;
	char	**args;
	char	**env_array;

	cmd_path = find_cmd_in_path(cmd->name, data->env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(cmd->name, 2);
		cleanup_and_exit(data, original_cmd, 127);
	}
	args = build_args_array(cmd);
	env_array = env_list_to_array(data->env);
	if (!args || !env_array)
		cleanup_and_exit(data, original_cmd, 1);
	execve(cmd_path, args, env_array);
	perror("minishell: execve");
	cleanup_and_exit(data, original_cmd, 127);
}

int	handle_pipe(t_cmd *current_cmd, t_token **tokens)
{
	if (!current_cmd || !tokens || !*tokens)
		return (-1);
	*tokens = (*tokens)->next;
	if (!*tokens)
	{
		ft_printf("syntax error near unexpected token `|'\n");
		return (-1);
	}
	if (init_next_cmd(current_cmd, tokens) == -1)
		return (-1);
	process_cmd_args(current_cmd->next, tokens);
	return (0);
}
