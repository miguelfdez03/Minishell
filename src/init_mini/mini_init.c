/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:15:17 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/09 16:40:54 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void init_tokens(t_token *token)
{
	token->value = NULL;
	token->type = EMPTY;
}

int init_data(t_data **data, char **env, t_env *env_t)
{
	*data = malloc(sizeof(t_data));
	if (!*data)
	{
		ft_printf("Error: Failed to allocate memory for data structure\n");
		return (-1);
	}
	(*data)->input = NULL;
	(*data)->cmd = NULL;
	(*data)->tokens = NULL;
	env_t = malloc(sizeof(t_env));
	if (!env_t)
	{
		ft_printf("Error: Failed to allocate memory for environment\n");
		free(*data);
		return (-1);
	}
	(*data)->env = env_t;
	if (init_env(env, env_t) == -1)
	{
		ft_printf("Error: Failed to initialize environment\n");
		free(env_t);
		free(*data);
		return (-1);
	}
	(*data)->path = NULL;
	(*data)->pipe_flag = -1;
	ft_printf("Data initialized\n");
	return (0);
}

/*int	main_loop(int argc, char **argv, char **env)
{
	char *input;

	input = NULL;
	while (1)
	{
		input = readline("spidershell>");
		if (!input)
		{
			ft_printf("exit");
			break;
			}
			if (ft_strlen(input) > 0)
			{
				if(input[0] == '\>' || input[0] == '\<' || input[0] == '\|')
					return (EXIT_FAILURE);
				else
				{
					lexer(input);
					add_history(input);
				}
			}
		}
	}
}
*/

int main_loop(int argc, char **argv, t_data **data, char **env)
{
	char *input;

	input = NULL;
	while (1)
	{
		input = readline("spidershell>");
		if (!input)
		{
			ft_printf("exit\n");
			break;
		}
		else
		{
			(*data)->cmd = parse_simple_input(input);
			(*data)->input = input;
			if ((*data)->cmd)
			{
				execute_command(*data);
				free_cmd((*data)->cmd);
				add_history(input);
			}
		}
		free(input);
	}
	return (0);
}