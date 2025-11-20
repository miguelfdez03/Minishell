#include "../minishell.h"

int	should_expand_heredoc(char *original_delimiter)
{
	int	i;

	i = 0;
	while (original_delimiter[i])
	{
		if (original_delimiter[i] == '"' || original_delimiter[i] == '\'')
			return (0);
		i++;
	}
	return (1);
}

static void	write_heredoc_content(int fd, char *clean_delim, int expand,
		t_data *data)
{
	t_heredoc_s	here_s;

	here_s.fd = fd;
	here_s.clean_delim = clean_delim;
	here_s.expand = expand;
	here_s.data = data;
	if (isatty(STDIN_FILENO))
		write_heredoc_interactive(&here_s);
	else
		write_heredoc_pipe(&here_s);
}

static int	open_and_write_heredoc(char *tmp_file, char *clean_delimiter,
		int expand, t_data *data)
{
	int	fd;

	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(clean_delimiter);
		return (-1);
	}
	fcntl(fd, F_SETFD, FD_CLOEXEC);
	write_heredoc_content(fd, clean_delimiter, expand, data);
	close(fd);
	return (0);
}

static int	redirect_heredoc_to_stdin(char *tmp_file)
{
	int	fd;

	fd = open(tmp_file, O_RDONLY);
	if (fd < 0)
		return (-1);
	fcntl(fd, F_SETFD, FD_CLOEXEC);
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	unlink(tmp_file);
	return (0);
}

static int	process_heredoc_file(char *tmp_file, char *clean_delim, int expand,
		t_data *data)
{
	extern volatile sig_atomic_t	g_signal_received;

	g_signal_received = 0;
	setup_signals_heredoc();
	if (open_and_write_heredoc(tmp_file, clean_delim, expand, data) == -1)
	{
		free(clean_delim);
		setup_signals_heredoc();
		return (-1);
	}
	free(clean_delim);
	setup_signals_heredoc();
	if (g_signal_received)
	{
		unlink(tmp_file);
		return (-2);
	}
	return (0);
}

int	handle_heredoc(char *delimiter, t_data *data)
{
	char	*clean_delimiter;
	int		expand;
	char	*tmp_file;
	int		result;

	tmp_file = "/tmp/.minishell_heredoc";
	clean_delimiter = remove_quotes_from_delimiter(delimiter);
	if (!clean_delimiter)
		return (-1);
	expand = should_expand_heredoc(delimiter);
	result = process_heredoc_file(tmp_file, clean_delimiter, expand, data);
	if (result == -2)
	{
		data->exit_status = 130;
		return (-2);
	}
	return (result);
}
