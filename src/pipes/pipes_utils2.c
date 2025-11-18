#include "../minishell.h"

void	cleanup_and_exit(t_data *data, t_cmd *original_cmd, int code)
{
	data->cmd = original_cmd;
	rl_clear_history();
	free_data(data);
	exit(code);
}

void	exec_external_cmd(t_data *data, t_cmd *cmd, t_cmd *original_cmd)
{
	char	*cmd_path;
	char	**args;
	char	**env_array;

	if (!cmd->name || cmd->name[0] == '\0')
	{
		ft_putendl_fd(": command not found", 2);
		cleanup_and_exit(data, original_cmd, 127);
	}
	cmd_path = find_cmd_in_path(cmd->name, data->env);
	if (cmd_path == (char *)-1)
		cleanup_and_exit(data, original_cmd, 126);
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

static int	wait_all_processes(int *exit_status, pid_t last_cmd_pid)
{
	int		status;
	pid_t	current_pid;
	int		sigint_received;

	sigint_received = 0;
	while (1)
	{
		current_pid = waitpid(-1, &status, 0);
		if (current_pid <= 0)
			break ;
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			sigint_received = 1;
		if (current_pid == last_cmd_pid)
		{
			if (WIFEXITED(status))
				*exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*exit_status = 128 + WTERMSIG(status);
		}
	}
	if (sigint_received)
		write(1, "\n", 1);
	return (*exit_status);
}

int	execute_pipeline(t_data *data)
{
	t_cmd	*current;
	int		input_fd;
	int		exit_status;
	pid_t	last_cmd_pid;

	current = data->cmd;
	input_fd = STDIN_FILENO;
	exit_status = 0;
	last_cmd_pid = -1;
	setup_signals_executing();
	while (current)
		process_pipeline_cmd(data, &current, &input_fd, &last_cmd_pid);
	wait_all_processes(&exit_status, last_cmd_pid);
	setup_signals_interactive();
	data->exit_status = exit_status;
	return (exit_status);
}

void	exec_cmd_in_child(t_data *data, t_cmd *cmd, int is_last_cmd)
{
	int		exit_code;
	t_cmd	*original_cmd;

	original_cmd = data->cmd;
	setup_signals_child();
	close_all_fds();
	data->cmd = cmd;
	if (apply_redirections(data) == -1)
		cleanup_and_exit(data, original_cmd, 1);
	if (cmd->builtin_id != BUILTIN_NONE)
	{
		exit_code = execute_builtin_by_id(data);
		cleanup_and_exit(data, original_cmd, exit_code);
	}
	exec_external_cmd(data, cmd, original_cmd);
}
