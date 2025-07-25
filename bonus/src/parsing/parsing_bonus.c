/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:25:26 by ahabdelr          #+#    #+#             */
/*   Updated: 2025/07/25 15:30:38 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	safe_free_textures(t_map *map)
{
	safe_free((void **) &map->data.wall);
	safe_free((void **) &map->data.door);
	safe_free((void **) &map->data.gun1);
	safe_free((void **) &map->data.gun2);
	safe_free((void **) &map->data.ceiling1);
	safe_free((void **) &map->data.ceiling2);
	safe_free((void **) &map->data.ceiling3);
	safe_free((void **) &map->data.hand);
	safe_free((void **) &map->data.floor1);
	safe_free((void **) &map->data.floor2);
	safe_free((void **) &map->data.floor3);
	safe_free((void **) &map->data.floor4);
	safe_free((void **) &map->data.floor5);
	safe_free((void **) &map->data.floor6);
	safe_free((void **) &map->data.floor7);
	safe_free((void **) &map->data.floor8);
}

// 1 = success, 0 = failure
int	map_parsing(char *file, t_map *map)
{
	int	gnl_calls;
	int	fd;

	gnl_calls = get_graphics(file, map);
	if (!gnl_calls)
		return (0);
	get_next_line(-1);
	fd = open(file, O_RDONLY);
	if (!get_map(file, gnl_calls, map))
	{
		safe_free((void **) &map->matrix);
		safe_free_textures(map);
		return (0);
	}
	close(fd);
	get_next_line(-1);
	fd = open(file, O_RDONLY);
	matrix_creation(map, fd, gnl_calls);
	return (1);
}
