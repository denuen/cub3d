/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 02:07:51 by apintaur          #+#    #+#             */
/*   Updated: 2025/07/25 14:35:19 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 15
#endif

#include "../includes/get_next_line_utils.h"
#include <stdlib.h>
#include <stdio.h>

char	*ft_createbuffer(int fd, char **join);
void	ft_freebuff(char *buffer);

char	*get_next_line(int fd)
{
	static char	*join;
	char		*buffer;
	ssize_t		i;

	buffer = ft_createbuffer(fd, &join);
	if (buffer == NULL)
		return (NULL);
	i = read (fd, buffer, BUFFER_SIZE);
	while (i > 0)
	{
		buffer[i] = '\0';
		join = ft_strnjoin(join, buffer, i);
		i = ft_strrlen(join, '\n');
		if (join[i] == '\n')
		{
			ft_freebuff (buffer);
			return (ft_strupdate(&join, i));
		}
		i = read (fd, buffer, BUFFER_SIZE);
	}
	ft_freebuff(buffer);
	if (join)
		return (ft_strupdate(&join, ft_strrlen(join, '\n')));
	return (NULL);
}

char	*ft_createbuffer(int fd, char **join)
{
	char	*new;

	if (fd < 0 || fd > FOPEN_MAX || BUFFER_SIZE <= 0)
	{
		if (join && *join)
		{
			free(*join);
			*join = NULL;
		}
		return (NULL);
	}
	new = (char *) malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!new)
		return (NULL);
	return (new);
}

void	ft_freebuff(char *buffer)
{
	if (buffer)
	{
		free (buffer);
		buffer = NULL;
	}
}
