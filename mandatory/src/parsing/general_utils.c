/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:39:28 by ahabdelr          #+#    #+#             */
/*   Updated: 2025/07/25 11:33:07 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_player(char c)
{
	if (c == 'W' || c == 'S' || c == 'E' || c == 'N')
		return (1);
	return (0);
}

int	color_helper(char *line, int j)
{
	int	commas;
	int	check;

	commas = 0;
	check = 0;
	while (line[j] != '\n' && line[j] != ' ' && line[j] != '\0')
	{
		if (ft_isnum(line[j]))
			check++;
		if (check >= 1 && check <= 3 && line[j] == ',')
		{
			check = 0;
			commas++;
		}
		j++;
	}
	if (commas != 2 || check == 0 || check > 3)
		return (1);
	return (0);
}

int	graphics_helper(char *line, int *gnl_calls, int *result, t_map *map)
{
	int	i;

	i = 0;
	while (line != NULL)
	{
		if (ft_strnstr(line, "NO", 2))
			result[1] = save_image(&i, &map->data.nt, line);
		else if (ft_strnstr(line, "SO", 2))
			result[2] = save_image(&i, &map->data.st, line);
		else if (ft_strnstr(line, "EA", 2))
			result[3] = save_image(&i, &map->data.et, line);
		else if (ft_strnstr(line, "WE", 2))
			result[4] = save_image(&i, &map->data.wt, line);
		else if (ft_strnstr(line, "c", 1))
			result[5] = save_color(&i, &map->data.ceiling, line);
		else if (ft_strnstr(line, "f", 1))
			result[6] = save_color(&i, &map->data.floor, line);
		safe_free((void **) &line);
		line = get_next_line(result[0]);
		if (i < 6)
			(*gnl_calls)++;
	}
	return (i);
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
		else if (is_player(line[i]))
			matrix[*j] = (int)line[i];
		i++;
		(*j)++;
	}
	if (line[i] == '\n' || line[i] == '\0')
	{
		while (i < map->sizes.map_lenght)
		{
			matrix[*j] = 2;
			(*j)++;
			i++;
		}
	}
}
