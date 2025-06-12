/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:32:29 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 17:35:47 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	is_valid_position(t_cub *cub, t_2ipoint pos)
{
	int	map_index;

	if (pos.x < 0 || pos.x >= cub->map.sizes.map_lenght
		|| pos.y < 0 || pos.y >= cub->map.sizes.map_height)
		return (0);
	map_index = pos.y * cub->map.sizes.map_lenght + pos.x;
	if (map_index < 0 || map_index >= (cub->map.sizes.map_height \
									* cub->map.sizes.map_lenght))
		return (0);
	return (cub->map.matrix[map_index] != WALL);
}

static int	close_x(t_cub *cub, t_2fpoint pos)
{
	t_2ipoint	map_pos;
	float		frac;
	int			next_pos;
	int			next_index;
	float		wall_margin;

	map_pos.x = (int)pos.x;
	map_pos.y = (int)pos.y;
	wall_margin = 0.1f;
	frac = pos.x - map_pos.x;
	if (frac < wall_margin || frac > (1.0f - wall_margin))
	{
		next_pos = map_pos.x + (-1 * (frac < wall_margin)) \
							+ (1 * (frac < wall_margin));
		if (next_pos >= 0 && next_pos < cub->map.sizes.map_lenght)
		{
			next_index = map_pos.y * cub->map.sizes.map_lenght
				+ next_pos;
			if (next_index >= 0 && next_index < cub->map.sizes.map_height
				* cub->map.sizes.map_lenght \
				&& cub->map.matrix[next_index] == WALL)
				return (1);
		}
	}
	return (0);
}

static int	close_y(t_cub *cub, t_2fpoint pos)
{
	t_2ipoint	map_pos;
	float		frac;
	int			next_pos;
	int			next_index;
	float		wall_margin;

	map_pos.x = (int)pos.x;
	map_pos.y = (int)pos.y;
	wall_margin = 0.1f;
	frac = pos.y - map_pos.y;
	if (frac < wall_margin || frac > (1.0f - wall_margin))
	{
		next_pos = map_pos.y + (-1 * (frac < wall_margin)) \
							+ (1 * (frac < wall_margin));
		if (next_pos >= 0 && next_pos < cub->map.sizes.map_height)
		{
			next_index = next_pos * cub->map.sizes.map_lenght
				+ map_pos.x;
			if (next_index >= 0 && next_index < cub->map.sizes.map_height
				* cub->map.sizes.map_lenght \
				&& cub->map.matrix[next_index] == WALL)
				return (1);
		}
	}
	return (0);
}

int	is_too_close_to_wall(t_cub *cub, t_2fpoint pos, int axis)
{
	if (axis == 0)
		return (close_x(cub, pos));
	return (close_y(cub, pos));
}

void	check_collisions(t_cub *cub, t_2fpoint new_pos, t_2fpoint old_pos)
{
	t_2ipoint	check_pos_x;
	t_2ipoint	check_pos_y;

	check_pos_x.x = (int)new_pos.x;
	check_pos_x.y = (int)old_pos.y;
	if (is_valid_position(cub, check_pos_x)
		&& !is_too_close_to_wall(cub, (t_2fpoint){new_pos.x, old_pos.y}, 0))
		cub->raycaster.player.pos.x = new_pos.x;
	check_pos_y.x = (int)cub->raycaster.player.pos.x;
	check_pos_y.y = (int)new_pos.y;
	if (is_valid_position(cub, check_pos_y)
		&& !is_too_close_to_wall(cub, (t_2fpoint){cub->raycaster.player.pos.x, \
																new_pos.y}, 1))
		cub->raycaster.player.pos.y = new_pos.y;
}
