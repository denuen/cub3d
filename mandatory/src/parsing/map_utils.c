/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 12:30:04 by ahabdelr          #+#    #+#             */
/*   Updated: 2025/07/25 15:20:02 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	safe_eject(char *line, char *prev, char *next)
{
	safe_free((void **)&line);
	safe_free((void **)&prev);
	safe_free((void **)&next);
	return (1);
}

int	check_helper(char *line, char *prev, char *next, int *player)
{
	int	i;
	int	not_first;

	i = 0;
	not_first = 0;
	while (line[i] != '\n' && line[i] != '\0')
	{
		if (line[i] == '1')
			not_first++;
		if (line[i] == '0')
		{
			if (!not_first)
				return (safe_eject(line, prev, next));
			if (surround_check(line, prev, next, i))
				return (safe_eject(line, prev, next));
		}
		if (is_player(line[i]))
			(*player)++;
		i++;
	}
	return (0);
}

void	sizes_helper(char *line, int fd, t_map *map)
{
	int	i;
	int	height;
	int	lenght;
	int	max_lenght;

	height = 0;
	max_lenght = 0;
	while (line != NULL)
	{
		i = 0;
		height++;
		lenght = 0;
		while (line[i] != '\n' && line[i] != '\0')
		{
			i++;
			lenght++;
		}
		if (lenght > max_lenght)
			max_lenght = lenght;
		safe_free((void **) &line);
		line = get_next_line(fd);
	}
	map->sizes.map_height = height;
	map->sizes.map_lenght = max_lenght;
}

char	*gnl_helper(char *line, int gnl_calls, int fd)
{
	int	i;

	i = 0;
	while (i < gnl_calls)
	{
		safe_free((void **) &line);
		line = get_next_line(fd);
		i++;
	}
	return (line);
}
