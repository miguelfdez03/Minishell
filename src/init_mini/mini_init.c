#include "../minishell.h"

void	free_data(t_data *data)
{
	if (!data)
		return ;
	if (data->env)
		free_env(data->env);
	if (data->tokens)
		free_tokens(data->tokens);
	if (data->cmd)
		free_cmd(data->cmd);
	if (data->input)
		free(data->input);
	if (data->path)
		free_string_array(data->path);
	free(data);
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
		}
		if ((*data)->tokens)
		{
			free_tokens((*data)->tokens);
			(*data)->tokens = NULL;
		}
		if ((*data)->cmd)
		{
			free_cmd((*data)->cmd);
			(*data)->cmd = NULL;
		}
		if (init_cmd_data(data) == -1)
			return ;
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
	return ((*data)->exit_status);
}
