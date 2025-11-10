#include "../minishell.h"

static void	exec_child_process(t_cmd *cmd, char *cmd_path,
		char **args, char **env_array)
{
	setup_signals_child();
	if (apply_redirections(cmd) == -1)
	{
		free(cmd_path);
		free(args);
		free_string_array(env_array);
		exit(1);
	}
	if (execve(cmd_path, args, env_array) == -1)
	{
		perror("minishell: execve");
		free(cmd_path);
		free(args);
		free_string_array(env_array);
		exit(127);
	}
}

static int	wait_and_cleanup(pid_t pid, char *cmd_path, char **args,
		char **env_array)
{
	int	status;

	waitpid(pid, &status, 0);
	free(cmd_path);
	free(args);
	free_string_array(env_array);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static int	prepare_execution(t_data *data, char **cmd_path, char ***args,
		char ***env_array)
{
	*cmd_path = find_cmd_in_path(data->cmd->name, data->env);
	if (!*cmd_path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(data->cmd->name, 2);
		return (127);
	}
	*args = build_args_array(data->cmd);
	*env_array = env_list_to_array(data->env);
	if (!*args || !*env_array)
	{
		free(*cmd_path);
		if (*args)
			free(*args);
		if (*env_array)
			free_string_array(*env_array);
		return (1);
	}
	return (0);
}

char	*find_cmd_in_path(char *cmd, t_env *env)
{
	char	**env_array;
	char	*cmd_path;
	char	*abs_path;

	if (!cmd || !env)
		return (NULL);
	abs_path = check_absolute_path(cmd);
	if (abs_path)
		return (abs_path);
	env_array = env_list_to_array(env);
	if (!env_array)
		return (NULL);
	cmd_path = find_command_path(cmd, env_array);
	free_string_array(env_array);
	return (cmd_path);
}

int	execute_external_command(t_data *data)
{
	pid_t	pid;
	char	*cmd_path;
	char	**args;
	char	**env_array;
	int		ret;

	ret = prepare_execution(data, &cmd_path, &args, &env_array);
	if (ret != 0)
		return (ret);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		free(cmd_path);
		free(args);
		free_string_array(env_array);
		return (1);
	}
	if (pid == 0)
		exec_child_process(data->cmd, cmd_path, args, env_array);
	return (wait_and_cleanup(pid, cmd_path, args, env_array));
}
