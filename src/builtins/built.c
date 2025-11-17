#include "../minishell.h"

int	execute_command(t_data *data)
{
	t_cmd	*cmd;
	int		exit_status;
	int		saved_stdin;
	int		saved_stdout;

	cmd = data->cmd;
	if (!cmd->name || cmd->name[0] == '\0')
	{
		if (!cmd->args || !cmd->args[0])
			return (0);
		cmd->name = ft_strdup(cmd->args[0]);
		cmd->builtin_id = identify_builtin(cmd->name);
	}
	if (cmd->next)
		return (execute_pipeline(data));
	if (cmd->builtin_id == BUILTIN_NONE)
	{
		exit_status = execute_external_command(data);
		data->exit_status = exit_status;
		return (exit_status);
	}
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(cmd) == -1)
		exit_status = 1;
	else
		exit_status = execute_builtin_by_id(data);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	data->exit_status = exit_status;
	return (exit_status);
}

static int	handle_basic_builtins(t_data *data)
{
	t_builtin_type	type;

	type = data->cmd->builtin_id;
	if (type == BUILTIN_CD)
		return (builtin_cd(data));
	if (type == BUILTIN_PWD)
		return (builtin_pwd(data));
	if (type == BUILTIN_EXIT)
		return (builtin_exit(data));
	if (type == BUILTIN_ECHO)
		return (builtin_echo(data));
	return (-1);
}

static int	handle_env_builtins(t_data *data)
{
	t_builtin_type	type;

	type = data->cmd->builtin_id;
	if (type == BUILTIN_ENV)
		return (builtin_env(data));
	if (type == BUILTIN_EXPORT)
		return (builtin_export(data));
	if (type == BUILTIN_UNSET)
		return (builtin_unset(data));
	return (-1);
}

int	execute_builtin_by_id(t_data *data)
{
	int	result;

	result = handle_basic_builtins(data);
	if (result != -1)
		return (result);
	result = handle_env_builtins(data);
	if (result != -1)
		return (result);
	printf("\nUnknown builtin\n");
	return (1);
}

int	builtin_pwd(t_data *data)
{
	char	*pwd;

	(void)data;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_putendl_fd("pwd: error getting current directory", 2);
		return (1);
	}
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (0);
}
