/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_collisions_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:36:03 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/14 11:32:50 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_valid_position(t_cub *cub, t_2ipoint pos)
{
	int	map_index;
	int	cell_value;

	if (pos.x < 0 || pos.x >= cub->map.sizes.map_lenght
		|| pos.y < 0 || pos.y >= cub->map.sizes.map_height)
		return (0);
	map_index = pos.y * cub->map.sizes.map_lenght + pos.x;
	if (map_index < 0 || map_index >= (cub->map.sizes.map_height \
							* cub->map.sizes.map_lenght))
		return (0);
	cell_value = cub->map.matrix[map_index];
	if (cell_value == WALL)
		return (0);
	if (cell_value == DOOR && !is_door_open(&cub->map, pos.x, pos.y))
		return (0);
	return (1);
}

static int	get_adjacent_cell_coord(t_2ipoint map_pos, float frac,
							float wall_margin, int axis)
{
	int	adjacent_coord;

	if (frac < wall_margin)
	{
		if (axis == 0)
			adjacent_coord = map_pos.x - 1;
		else
			adjacent_coord = map_pos.y - 1;
	}
	else
	{
		if (axis == 0)
			adjacent_coord = map_pos.x + 1;
		else
			adjacent_coord = map_pos.y + 1;
	}
	return (adjacent_coord);
}

static int	is_adjacent_cell_blocking(t_cub *cub, t_2ipoint map_pos,
								int adjacent_coord, int axis)
{
	int			map_index;
	t_2ipoint	check;

	if (adjacent_coord < 0 || adjacent_coord >= cub->map.sizes.map_lenght)
		return (0);
	if (axis == 0)
		check = (t_2ipoint){adjacent_coord, map_pos.y};
	else
		check = (t_2ipoint){map_pos.x, adjacent_coord};
	map_index = check.y * cub->map.sizes.map_lenght + check.x;
	if (map_index < 0 || map_index >= (cub->map.sizes.map_height \
		* cub->map.sizes.map_lenght))
		return (0);
	if (cub->map.matrix[map_index] == WALL)
		return (1);
	if (cub->map.matrix[map_index] == DOOR
		&& !is_door_open(&cub->map, check.x, check.y))
		return (1);
	return (0);
}

static int	is_too_close_to_wall(t_cub *cub, t_2fpoint pos, int axis)
{
	t_2ipoint	map_pos;
	float		frac;
	float		wall_margin;
	int			adjacent_coord;

	map_pos = (t_2ipoint){pos.x, pos.y};
	wall_margin = 0.1f;
	if (axis == 0)
		frac = pos.x - map_pos.x;
	else
		frac = pos.y - map_pos.y;
	if (frac < wall_margin || frac > (1.0f - wall_margin))
	{
		adjacent_coord = get_adjacent_cell_coord(map_pos, frac, \
			wall_margin, axis);
		if (is_adjacent_cell_blocking(cub, map_pos, adjacent_coord, axis))
			return (1);
	}
	return (0);
}

void	check_collisions(t_cub *cub, t_2fpoint new_pos, t_2fpoint old_pos)
{
	t_2ipoint	check_pos_x;
	t_2ipoint	check_pos_y;

	check_pos_x.x = (int)new_pos.x;
	check_pos_x.y = (int)old_pos.y;
	if (is_valid_position(cub, check_pos_x) && !is_too_close_to_wall(cub, \
		(t_2fpoint){new_pos.x, old_pos.y}, 0))
		cub->raycaster.player.pos.x = new_pos.x;
	check_pos_y.x = (int)cub->raycaster.player.pos.x;
	check_pos_y.y = (int)new_pos.y;
	if (is_valid_position(cub, check_pos_y) && !is_too_close_to_wall(cub, \
		(t_2fpoint){cub->raycaster.player.pos.x, new_pos.y}, 1))
		cub->raycaster.player.pos.y = new_pos.y;
}
