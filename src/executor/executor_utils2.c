#include "../minishell.h"

int	handle_fork_error(char *cmd_path, char **args, char **env_array)
{
	perror("minishell: fork");
	free(cmd_path);
	free(args);
	free_string_array(env_array);
	setup_signals_interactive();
	return (1);
}

int	wait_and_cleanup(pid_t pid, char *cmd_path, char **args,
		char **env_array)
{
	int	status;

	waitpid(pid, &status, 0);
	free(cmd_path);
	free(args);
	free_string_array(env_array);
	if ((status & 0x7f) != 0)
		return (128 + (status & 0x7f));
	return (1);
}
