/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miguel-f <miguel-f@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:11:38 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/07 14:35:54 by miguel-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	new_redir->file = value;
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
