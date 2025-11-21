#include "../minishell.h"

void	setup_heredoc_io(int *saved_stdout)
{
	*saved_stdout = dup(STDOUT_FILENO);
	dup2(STDERR_FILENO, STDOUT_FILENO);
}

void	write_heredoc_interactive(t_heredoc_s *here_s)
{
	char	*line;
	int		saved_stdout;
	int		stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	setup_signals_heredoc();
	setup_heredoc_io(&saved_stdout);
	while (1)
	{
		line = readline("> ");
		if (check_heredoc_exit(line, here_s->clean_delim))
			break ;
		process_heredoc_line_interactive(here_s, line);
	}
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
}

void	process_buffer_line(t_heredoc_s *here_s, char *buffer)
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

int	process_pipe_newline(t_heredoc_s *here_s, char *buffer)
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
