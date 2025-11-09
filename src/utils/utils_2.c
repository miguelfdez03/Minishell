#include "../minishell.h"

int	ft_word_length(char *line, int i)
{
	while (line[i] && is_space(line[i]) != EXIT_SUCCESS
		&& line[i] != '|' && line[i] != '<' && line[i] != '>'
		&& line[i] != '\'' && line[i] != '"')
		i++;
	return (i);
}

int	ft_strcmp2(char *str1, char *str2)
{
	int	i;

	if (!str1 || !str2)
		return (-1);
	i = 0;
	while (str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}
