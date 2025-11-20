#include "../minishell.h"

static void	copy_quoted_content(char *delimiter, int *i, char *result, int *j)
{
	char	quote;

	quote = delimiter[(*i)++];
	while (delimiter[*i] && delimiter[*i] != quote)
		result[(*j)++] = delimiter[(*i)++];
	if (delimiter[*i] == quote)
		(*i)++;
}

char	*remove_quotes_from_delimiter(char *delimiter)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(ft_strlen(delimiter) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '"' || delimiter[i] == '\'')
			copy_quoted_content(delimiter, &i, result, &j);
		else
			result[j++] = delimiter[i++];
	}
	result[j] = '\0';
	return (result);
}

void	process_heredoc_line_interactive(t_heredoc_s *here_s, char *line)
{
	char	*expanded;

	if (here_s->expand)
	{
		expanded = expand_string(line, here_s->data->env,
				here_s->data->exit_status);
		free(line);
		if (expanded)
			line = expanded;
		else
			line = ft_strdup("");
	}
	if (line)
	{
		write(here_s->fd, line, ft_strlen(line));
		write(here_s->fd, "\n", 1);
		free(line);
	}
}

int	check_heredoc_exit(char *line, char *clean_delim)
{
	extern volatile sig_atomic_t	g_signal_received;

	if (g_signal_received == 130)
		return (1);
	if (!line || g_signal_received)
	{
		if (!g_signal_received)
			ft_putstr_fd("minishell: warning: heredoc EOF\n", 2);
		if (line)
			free(line);
		return (1);
	}
	if (ft_strcmp2(line, clean_delim) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}
