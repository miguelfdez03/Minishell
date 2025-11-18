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
	if (isatty(STDIN_FILENO))
		write_heredoc_interactive(fd, clean_delim, expand, data);
	else
		write_heredoc_pipe(fd, clean_delim, expand, data);
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
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	unlink(tmp_file);
	return (0);
}

int	handle_heredoc(char *delimiter, t_data *data)
{
	char	*clean_delimiter;
	int		expand;
	char	*tmp_file;

	tmp_file = "/tmp/.minishell_heredoc";
	clean_delimiter = remove_quotes_from_delimiter(delimiter);
	if (!clean_delimiter)
		return (-1);
	expand = should_expand_heredoc(delimiter);
	setup_signals_heredoc();
	if (open_and_write_heredoc(tmp_file, clean_delimiter, expand, data) == -1)
	{
		setup_signals_interactive();
		return (-1);
	}
	free(clean_delimiter);
	if (redirect_heredoc_to_stdin(tmp_file) == -1)
	{
		setup_signals_interactive();
		return (-1);
	}
	setup_signals_interactive();
	return (0);
}
