/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:15:17 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/24 19:10:02 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_tokens(t_token *token)
{
	token->value = NULL;
	token->type = EMPTY;
}

int	init_cmd_data(t_data **data)
{
	(*data)->cmd = malloc(sizeof(t_cmd));
	if (!(*data)->cmd)
	{
		ft_printf("Error: Failed to allocate memory for cmd structure\n");
		return (-1);
	}
	(*data)->cmd->name = NULL;
	(*data)->cmd->args = NULL;
	(*data)->cmd->builtin_id = BUILTIN_NONE;
	(*data)->cmd->redirections = NULL;
	(*data)->cmd->next = NULL;
	return (0);
}

int	init_data(t_data **data, char **env, t_env *env_t)
{
	*data = malloc(sizeof(t_data));
	if (!*data)
		return (-1);
	(*data)->input = NULL;
	(*data)->tokens = NULL;
	env_t = malloc(sizeof(t_env));
	if (!env_t)
	{
		free(*data);
		return (-1);
	}
	(*data)->env = env_t;
	if (init_env(env, env_t) == -1)
	{
		free(env_t);
		free(*data);
		return (-1);
	}
	if (init_cmd_data(data) == -1)
	{
		free(env_t);
		free(*data);
		return (-1);
	}
	(*data)->path = NULL;
	(*data)->pipe_flag = -1;
	(*data)->exit_status = 0;
	return (0);
}

static char	*read_multiline(char *input)
{
	char	quote;
	char	*cont;
	char	*temp;

	quote = check_unclosed_quotes(input);
	while (quote != 0)
	{
		cont = readline("> ");
		if (!cont)
		{
			ft_printf("syntax error: unexpected end of file\n");
			free(input);
			return (NULL);
		}
		temp = ft_strjoin(input, "\n");
		free(input);
		input = ft_strjoin(temp, cont);
		free(temp);
		free(cont);
		quote = check_unclosed_quotes(input);
	}
	return (input);
}

static void	process_input(char *input, t_data **data)
{
	if (ft_strlen(input) > 0)
	{
		if (lexer(input, data) >= 0)
		{
			execute_command(*data);
			if ((*data)->tokens)
			{
				free_tokens((*data)->tokens);
				(*data)->tokens = NULL;
			}
			if ((*data)->cmd)
				free_cmd((*data)->cmd);
		}
		init_cmd_data(data);
	}
}

int	main_loop(int argc, char **argv, t_data **data)
{
	char	*input;

	while (1)
	{
		input = readline("spidershell> ");
		if (!input)
		{
			ft_printf("exit\n");
			break ;
		}
		input = read_multiline(input);
		if (!input)
			continue ;
		add_history(input);
		process_input(input, data);
		free(input);
	}
	return (0);
}
