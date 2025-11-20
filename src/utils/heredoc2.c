#include "../minishell.h"

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
