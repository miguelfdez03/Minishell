/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 10:42:13 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/10/13 11:54:38 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Print all tokens (for debugging)
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int		i;
	char	*type_names[] = {"WORD", "STRING", "PIPE", "REDIR_IN", 
		"REDIR_OUT", "REDIR_APPEND", "HEREDOC", "EMPTY"};

	current = tokens;
	i = 0;
	ft_printf("\n=== TOKEN LIST ===\n");
	while (current)
	{
		ft_printf("Token[%d]: type=%s, value='%s'\n", 
			i, type_names[current->type], 
			current->value ? current->value : "(null)");
		current = current->next;
		i++;
	}
	ft_printf("==================\n\n");
}

// Free all tokens in the list
void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

// Count number of tokens
int	count_tokens(t_token *tokens)
{
	t_token	*current;
	int		count;

	current = tokens;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

// Get token at specific index
t_token	*get_token_at(t_token *tokens, int index)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (current && i < index)
	{
		current = current->next;
		i++;
	}
	return (current);
}
