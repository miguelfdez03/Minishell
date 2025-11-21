/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:42:31 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/21 08:44:39 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	copy_quoted_content(char *str, char *result, int *i, int *j)
{
	char	quote;

	quote = str[(*i)++];
	while (str[*i] && str[*i] != quote)
		result[(*j)++] = str[(*i)++];
	if (str[*i] == quote)
		(*i)++;
}

static char	*process_quotes_in_path(char *str)
{
	char	*result;
	int		i;
	int		j;

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
			copy_quoted_content(str, result, &i, &j);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	free(str);
	return (result);
}

void	add_redir(t_redir **redir, t_token_type type, char *value)
{
	t_redir	*new_redir;
	t_redir	*current;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->type = type;
	if (type == HEREDOC)
		new_redir->file = value;
	else
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
