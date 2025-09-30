/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:21:46 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/09/30 17:34:38 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_tokens(t_token *token)
{
	token->value = NULL;
	token->type = EMPTY;	
}

void init_data(t_data *data, char **env, t_env *env_t)
{
    data->cmd = NULL;
    data->env = init_env(env, env_t);
    data->path = NULL;
    data->pipe_flag = -1;
    data->t_lexer = NULL;
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
					*/
				
int	main_loop(int argc, char **argv, t_data **data, char **env)
{
	char	*input;
	t_cmd	*cmd;
	
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
			cmd = parse_simple_input(input);
			(*data)->input = input;
			if (cmd)
			{
				printf("🕷️ Command: %s, Type: %s, Builtin ID: %d\n", 
					cmd->name, 
					(cmd->type == CMD_BUILTIN) ? "BUILTIN" : "EXTERNAL",
					cmd->builtin_id);           
				execute_command(cmd, env);
				free_cmd(cmd);
				add_history(input);
			}
		}
    	free(input);
	}
    return (0);
}