#include "../minishell.h"

char	*remove_quotes_from_delimiter(char *delimiter)
{
	char	*result;
	int		i;
	int		j;
	char	quote;

	result = malloc(ft_strlen(delimiter) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '"' || delimiter[i] == '\'')
		{
			quote = delimiter[i++];
			while (delimiter[i] && delimiter[i] != quote)
				result[j++] = delimiter[i++];
			if (delimiter[i] == quote)
				i++;
		}
		else
			result[j++] = delimiter[i++];
	}
	result[j] = '\0';
	return (result);
}

static void	process_heredoc_line_interactive(int fd, char *line, int expand,
		t_data *data)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_string(line, data->env, data->exit_status);
		free(line);
		line = expanded;
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

void	write_heredoc_interactive(int fd, char *clean_delim, int expand,
		t_data *data)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp2(line, clean_delim) == 0)
		{
			if (!line)
				ft_putstr_fd("minishell: warning: heredoc EOF\n", 2);
			free(line);
			break ;
		}
		process_heredoc_line_interactive(fd, line, expand, data);
	}
}

static void	process_buffer_line(int fd, char *buffer, int expand, t_data *data)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_string(buffer, data->env, data->exit_status);
		write(fd, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
		write(fd, buffer, ft_strlen(buffer));
	write(fd, "\n", 1);
}

void	write_heredoc_pipe(int fd, char *clean_delim, int expand, t_data *data)
{
	char	buffer[10000];
	ssize_t	bytes_read;
	int		i;

	i = 0;
	bytes_read = read(STDIN_FILENO, buffer + i, 1);
	while (bytes_read > 0)
	{
		if (buffer[i] == '\n')
		{
			buffer[i] = '\0';
			if (ft_strcmp2(buffer, clean_delim) == 0)
				break ;
			process_buffer_line(fd, buffer, expand, data);
			i = 0;
		}
		else
			i++;
		bytes_read = read(STDIN_FILENO, buffer + i, 1);
	}
}
