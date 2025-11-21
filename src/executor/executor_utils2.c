/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:41:31 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/21 08:41:32 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(2, "Quit (core dumped)\n", 19);
		return (128 + WTERMSIG(status));
	}
	return (1);
}
