#include "../minishell.h"

static int	execute_builtin_with_redir(t_data *data)
{
	int	exit_status;
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(data) == -1)
		exit_status = 1;
	else
		exit_status = execute_builtin_by_id(data);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (exit_status);
}

static int	handle_empty_cmd(t_cmd *cmd, t_data *data)
{
	if (!cmd->args || !cmd->args[0])
	{
		if (cmd->next || cmd->redirections)
		{
			ft_putendl_fd(": command not found", 2);
			data->exit_status = 127;
			return (data->exit_status);
		}
		return (0);
	}
	cmd->name = ft_strdup(cmd->args[0]);
	cmd->builtin_id = identify_builtin(cmd->name);
	return (-1);
}

int	execute_command(t_data *data)
{
	t_cmd	*cmd;
	int		exit_status;
	int		saved_stdin;
	int		saved_stdout;

	cmd = data->cmd;
	if ((!cmd->name || cmd->name[0] == '\0') && (!cmd->args || !cmd->args[0]))
	{
		if (cmd->redirections)
		{
			saved_stdin = dup(STDIN_FILENO);
			saved_stdout = dup(STDOUT_FILENO);
			exit_status = apply_redirections(data);
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
			if (exit_status == -1)
				return (1);
			return (0);
		}
		return (0);
	}
	if (!cmd->name || cmd->name[0] == '\0')
	{
		exit_status = handle_empty_cmd(cmd, data);
		if (exit_status != -1)
			return (exit_status);
	}
	if (cmd->next)
		return (execute_pipeline(data));
	if (cmd->builtin_id == BUILTIN_NONE)
	{
		exit_status = execute_external_command(data);
		data->exit_status = exit_status;
		return (exit_status);
	}
	exit_status = execute_builtin_with_redir(data);
	data->exit_status = exit_status;
	return (exit_status);
}

int	execute_builtin_by_id(t_data *data)
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
	if (type == BUILTIN_ENV)
		return (builtin_env(data));
	if (type == BUILTIN_EXPORT)
		return (builtin_export(data));
	if (type == BUILTIN_UNSET)
		return (builtin_unset(data));
	printf("\nUnknown builtin\n");
	return (1);
}
