/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:39:28 by ahabdelr          #+#    #+#             */
/*   Updated: 2025/07/25 15:28:23 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_player(char c)
{
	if (c == 'W' || c == 'S' || c == 'E' || c == 'N')
		return (1);
	return (0);
}

void	matrix_helper(char *line, int *matrix, int *j, t_map *map)
{
	int	i;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
	{
		if (line[i] == ' ')
			matrix[*j] = 2;
		else if (line[i] == '0')
			matrix[*j] = 0;
		else if (line[i] == '1')
			matrix[*j] = 1;
		else if (line[i] == 'D')
			matrix[*j] = 3;
		else if (is_player(line[i]))
			matrix[*j] = (int)line[i];
		i++;
		(*j)++;
	}
	if (line[i] == '\n' || line[i] == '\0')
	{
		while (i++ < map->sizes.map_lenght)
			matrix[(*j)++] = 2;
	}
}
