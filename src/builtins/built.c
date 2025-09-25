/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel <miguel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 19:29:57 by miguel            #+#    #+#             */
/*   Updated: 2025/09/25 17:08:06 by miguel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_command(t_cmd *cmd, char **envp)
{
	if (cmd->type == CMD_EXTERNAL)
		printf("external command \n");
	else
		execute_builtin_by_id(cmd, envp);
	return (0);
}

// Ejecutar built-in
static int	handle_basic_builtins(t_builtin_type type)
{
	if (type == BUILTIN_CD)
		return (printf("\nbuiltin cd\n"), 0);
	else if (type == BUILTIN_PWD)
		return (printf("\nbuiltin pwd\n"), 0);
	else if (type == BUILTIN_EXIT)
		return (printf("\nbuiltin exit\n"), 0);
	else if (type == BUILTIN_ECHO)
		return (printf("\nbuiltin echo\n"), 0);
	return (-1);
}

static int	handle_env_builtins(t_builtin_type type)
{
	if (type == BUILTIN_ENV)
		return (printf("\nbuiltin env\n"), 0);
	else if (type == BUILTIN_EXPORT)
		return (printf("\nbuiltin export\n"), 0);
	else if (type == BUILTIN_UNSET)
		return (printf("\nbuiltin unset\n"), 0);
	return (-1);
}

int	execute_builtin_by_id(t_cmd *cmd, char **envp)
{
	int	result;

	result = handle_basic_builtins(cmd->builtin_id);
	if (result != -1)
		return (result);
	result = handle_env_builtins(cmd->builtin_id);
	if (result != -1)
		return (result);
	printf("\nUnknown builtin\n");
	return (1);
}