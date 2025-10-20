/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:04:28 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/19 20:08:42 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Initialize a command structure with default values
void	init_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	cmd->name = NULL;
	cmd->args = NULL;
	cmd->builtin_id = BUILTIN_NONE;
	cmd->redirections = NULL;
	cmd->next = NULL;
}

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
	// Inicializar estructura con valores por defecto
	init_cmd(cmd);
	// Asignar nombre del comando
	cmd->name = ft_strdup(cmd_name);
	if (!cmd->name)
	{
		free(cmd);
		return (NULL);
	}
	// Identificar si es built-in
	builtin_id = identify_builtin(cmd_name);
	if (builtin_id != BUILTIN_NONE)
		cmd->builtin_id = builtin_id;
	else
		cmd->builtin_id = BUILTIN_NONE;
	return (cmd);
}

// Añadir argumentos al comando
// Cuenta los argumentos actuales en cmd->args
int	count_cmd_args(t_cmd *cmd)
{
	int	count;

	count = 0;
	if (cmd && cmd->args)
	{
		while (cmd->args[count])
			count++;
	}
	return (count);
}

// Añade un argumento al comando
void	add_cmd_arg(t_cmd *cmd, char *arg)
{
	int		i;
	int		count;
	char	**new_args;

	if (!cmd || !arg)
		return ;
	count = count_cmd_args(cmd);
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
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
/*	if (cmd->redirections)
		free_redirs(cmd->redirections); */
	free(cmd);
}

/* Procesa una redireccion y avanza el puntero */
static t_token	*process_redir(t_token *current, t_cmd *cmd)
{
	t_token_type	redir_type;

	if (!current || !current->next)
		return (current);
	redir_type = current->type;
	current = current->next;
	if (current->type == WORD || current->type == STRING)
		add_redir(&(cmd->redirections), redir_type, ft_strdup(current->value));
	return (current);
}

/* Convierte lista de tokens en comando */
t_cmd	*tokens_to_cmd(t_token *tokens)
{
	t_cmd	*cmd;
	t_token	*current;

	if (!tokens)
		return (NULL);
	cmd = create_cmd(tokens->value);
	if (!cmd)
		return (NULL);
	current = tokens->next;
	while (current)
	{
		if (current->type == WORD || current->type == STRING)
			add_cmd_arg(cmd, current->value);
		else if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == REDIR_APPEND || current->type == HEREDOC)
			current = process_redir(current, cmd);
		else if (current->type == PIPE)
			break ;
		current = current->next;
	}
	return (cmd);
}
