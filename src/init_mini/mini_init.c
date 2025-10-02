/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 12:07:32 by miguel-f          #+#    #+#             */
/*   Updated: 2025/10/02 13:59:35 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_tokens(t_token *token)
{
	token->value = NULL;
	token->type = EMPTY;	
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

int	main_loop(int argc, char **argv, char **env)
{
    char	*input;
    t_cmd	*cmd;
    
    input = NULL;
    while (1)
    {
        input = readline("spidershell>");
        if (!input)
        {
          cmd = create_cmd("exit");
          builtin_exit(cmd);
        }
        if (ft_strlen(input) > 0)
        {
            // Parsear input simple
            cmd = parse_simple_input(input);
            if (cmd)
            {
                execute_command(cmd, env);
                free_cmd(cmd);
            }
            add_history(input);
        }
        free(input);
    }
    return (0);
}