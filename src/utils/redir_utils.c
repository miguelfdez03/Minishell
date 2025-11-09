#include "../minishell.h"

static char	*process_quotes_in_path(char *str)
{
	char	*result;
	int		i;
	int		j;
	char	quote;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (str);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				result[j++] = str[i++];
			if (str[i] == quote)
				i++;
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	free(str);
	return (result);
}

int	ft_redir_length(char *line, int i, t_token_type type)
{
	if (type == 2 || type == 3 || type == 4)
		i++;
	else if (type == 5 || type == 6)
		i = i + 2;
	else
		return (-1);
	while ((ft_isalpha(line[i]) == 1
			|| line[i] == 46) && is_space(line[i]) != EXIT_SUCCESS)
		i++;
	return (i);
}

void	add_redir(t_redir **redir, t_token_type type, char *value)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->type = type;
	new_redir->file = process_quotes_in_path(value);
	new_redir->next = NULL;
	if (!*redir)
	{
		*redir = new_redir;
		return ;
	}
	current = *redir;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

void	free_redirs(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = tmp;
	}
}
