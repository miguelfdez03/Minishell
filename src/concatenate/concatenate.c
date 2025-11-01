/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concatenate.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 13:23:51 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/11/01 17:30:03 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	should_concatenate(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == PIPE || token->type == REDIR_APPEND
		|| token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == HEREDOC)
		return (0);
	return (1);
}

static char	*join_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	return (res);
}

static void	remove_tok_list(t_token *prev, t_token *current)
{
	prev->next = current->next;
	free(current->value);
	free(current);
}

void	concatenate_tokens(t_token **tokens)
{
	t_token	*prev;
	t_token	*current;
	t_token	*temp;

	if (!tokens || !*tokens || !(*tokens)->next)
		return ;
	prev = *tokens;
	current = prev->next;
	while (current)
	{
		if (current->space == 0 && should_concatenate(prev) == 1
			&& should_concatenate(current) == 1)
		{
			prev->value = join_free(prev->value, current->value);
			temp = current->next;
			remove_tok_list(prev, current);
			current = temp;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}