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

static void	process_heredoc_line_interactive(t_heredoc_s *here_s, char *line)
{
	char	*expanded;

	if (here_s->expand)
	{
		expanded = expand_string(line, here_s->data->env, here_s->data->exit_status);
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

static int	check_heredoc_exit(char *line, char *clean_delim)
{
	extern volatile sig_atomic_t	g_signal_received;

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

static void	setup_heredoc_io(int *saved_stdout)
{
	signal(SIGPIPE, SIG_IGN);
	rl_catch_signals = 0;
	*saved_stdout = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
}

void	write_heredoc_interactive(t_heredoc_s *here_s)
{
	char	*line;
	int		saved_stdout;

	setup_heredoc_io(&saved_stdout);
	while (1)
	{
		line = readline("> ");
		if (check_heredoc_exit(line, here_s->clean_delim))
			break ;
		process_heredoc_line_interactive(here_s, line);
	}
	rl_catch_signals = 1;
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
}

static void	process_buffer_line(t_heredoc_s *here_s, char *buffer)
{
	char	*expanded;

	if (here_s->expand)
	{
		expanded = expand_string(buffer, here_s->data->env,
				here_s->data->exit_status);
		if (expanded)
		{
			write(here_s->fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
	}
	else
		write(here_s->fd, buffer, ft_strlen(buffer));
	write(here_s->fd, "\n", 1);
}

static int	process_pipe_newline(t_heredoc_s *here_s, char *buffer)
{
	if (ft_strcmp2(buffer, here_s->clean_delim) == 0)
		return (1);
	process_buffer_line(here_s, buffer);
	return (0);
}

void	write_heredoc_pipe(t_heredoc_s *here_s)
{
	char	buffer[10000];
	ssize_t	bytes_read;
	int		i;

	i = 0;
	while (1)
	{
		bytes_read = read(STDIN_FILENO, buffer + i, 1);
		if (bytes_read <= 0 || i >= 9999)
			break ;
		if (buffer[i] == '\n')
		{
			buffer[i] = '\0';
			if (process_pipe_newline(here_s, buffer))
				break ;
			ft_memset(buffer, 0, i + 1);
			i = 0;
		}
		else
			i++;
	}
}
