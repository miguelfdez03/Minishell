/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:04:28 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/02 12:04:31 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_builtin_type	identify_builtin(char *cmd)
{
	if (!cmd)
		return (BUILTIN_NONE);
	if (ft_strcmp2(cmd, "cd") == 0)
		return (BUILTIN_CD);
	if (ft_strcmp2(cmd, "pwd") == 0)
		return (BUILTIN_PWD);
	if (ft_strcmp2(cmd, "exit") == 0)
		return (BUILTIN_EXIT);
	if (ft_strcmp2(cmd, "echo") == 0)
		return (BUILTIN_ECHO);
	if (ft_strcmp2(cmd, "env") == 0)
		return (BUILTIN_ENV);
	if (ft_strcmp2(cmd, "export") == 0)
		return (BUILTIN_EXPORT);
	if (ft_strcmp2(cmd, "unset") == 0)
		return (BUILTIN_UNSET);
	return (BUILTIN_NONE);
}

// Crear nueva estructura de comando
t_cmd	*create_cmd(char *cmd_name)
{
	t_cmd			*cmd;
	t_builtin_type	builtin_id;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	// Inicializar estructura
	cmd->name = ft_strdup(cmd_name);
	cmd->args = NULL; // Se llenará después
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->next = NULL;
	// Identificar si es built-in
	builtin_id = identify_builtin(cmd_name);
	if (builtin_id != BUILTIN_NONE)
	{
		cmd->type = CMD_BUILTIN;
		cmd->builtin_id = builtin_id;
	}
	else
	{
		cmd->type = CMD_EXTERNAL;
		cmd->builtin_id = BUILTIN_NONE;
	}
	return (cmd);
}

// Añadir argumentos al comando
void	add_cmd_arg(t_cmd *cmd, char *arg)
{
	int		i;
	int		count;
	char	**new_args;

	if (!cmd || !arg)
		return ;
	// Contar argumentos actuales
	count = 0;
	if (cmd->args)
	{
		while (cmd->args[count])
			count++;
	}
	// Crear nuevo array con espacio para un argumento más + NULL
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	// Copiar argumentos existentes
	i = 0;
	if (cmd->args)
	{
		while (i < count)
		{
			new_args[i] = cmd->args[i];
			i++;
		}
		free(cmd->args);
	}
	// Añadir nuevo argumento
	new_args[i] = ft_strdup(arg);
	new_args[i + 1] = NULL;
	cmd->args = new_args;
}

// Liberar memoria de comando
void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->name)
		free(cmd->name);
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	free(cmd);
}
