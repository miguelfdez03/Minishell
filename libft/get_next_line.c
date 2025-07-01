/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lruiz-to <lruiz-to@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 11:01:37 by lruiz-to          #+#    #+#             */
/*   Updated: 2025/02/06 13:06:24 by lruiz-to         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_read_line(int fd, char *storage)
{
	int		checker;
	char	*line;

	line = ft_calloc_gnl(sizeof(char), (BUFFER_SIZE + 1));
	if (!line)
		return (NULL);
	checker = 1;
	while (!(ft_strchr_gnl(storage, '\n')) && checker != 0)
	{
		checker = read(fd, line, BUFFER_SIZE);
		if (checker < 0)
		{
			free(line);
			free(storage);
			return (NULL);
		}
		line[checker] = 0;
		storage = ft_strjoin_gnl(storage, line);
	}
	free(line);
	return (storage);
}

static char	*ft_next_line(char *storage)
{
	int		index;
	char	*line;

	index = 0;
	if (storage[0] == 0)
		return (NULL);
	while (storage[index] && (storage[index] != '\n'))
		index++;
	line = malloc(sizeof(char) * (index + 2));
	if (!line)
		return (NULL);
	index = 0;
	while (storage[index] && (storage[index] != '\n'))
	{
		line[index] = storage[index];
		index++;
	}
	if (storage[index] == '\n')
	{
		line[index] = storage[index];
		index++;
	}
	line[index] = '\0';
	return (line);
}

static char	*ft_get_the_rest(char *storage)
{
	int		index;
	int		rest_index;
	char	*rest;	

	index = 0;
	while (storage[index] && (storage[index] != '\n'))
		index++;
	if (!storage[index])
	{
		free(storage);
		return (NULL);
	}
	rest = malloc(sizeof(char) * (ft_strlen_gnl(storage) - index + 1));
	if (!rest)
	{
		free(rest);
		return (NULL);
	}
	++index;
	rest_index = 0;
	while (storage[index])
		rest[rest_index++] = storage[index++];
	rest[rest_index] = '\0';
	free(storage);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*storage;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	storage = ft_read_line(fd, storage);
	if (!storage)
		return (NULL);
	line = ft_next_line(storage);
	storage = ft_get_the_rest(storage);
	return (line);
}
