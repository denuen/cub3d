/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 08:21:02 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 19:27:07 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include <math.h>

static void	draw_minimap_square(t_image *img, t_2ipoint start,
				int size, unsigned int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			if ((start.x + x >= 0 && start.x + x < SCREEN_WIDTH) && \
				(start.y + y >= 0 && start.y + y < SCREEN_HEIGHT))
				mymlx_pixel_put(img, start.x + x, start.y + y, color);
			x++;
		}
		y++;
	}
}

static void	cast_minimap_fov_ray(t_cub *cub, t_2ipoint base_offset, float angle)
{
	t_2fpoint	ray_dir;
	t_2fpoint	curr_pmap;
	t_2ipoint	check;
	int			steps;

	ray_dir = (t_2fpoint){cosf(angle) * 0.03f, sinf(angle) * 0.03f};
	curr_pmap = cub->raycaster.player.pos;
	steps = 0;
	while (steps++ < MINIMAP_CELL_SIZE * 5)
	{
		check.x = base_offset.x + (int)(curr_pmap.x * MINIMAP_CELL_SIZE);
		check.y = base_offset.y + (int)(curr_pmap.y * MINIMAP_CELL_SIZE);
		if ((check.x >= 0 && check.x < SCREEN_WIDTH)
			&& (check.y >= 0 && check.y < SCREEN_HEIGHT))
			mymlx_pixel_put(&cub->pic.img, check.x, check.y, FOV_FILL_COLOR);
		if ((int)curr_pmap.x < 0 || (int)curr_pmap.x >= \
			cub->map.sizes.map_lenght || (int)curr_pmap.y < 0 \
			|| (int)curr_pmap.y >= cub->map.sizes.map_height)
			break ;
		if (cub->map.matrix[(int)curr_pmap.y * cub->map.sizes.map_lenght + \
			(int)curr_pmap.x] == WALL)
			break ;
		curr_pmap.x += ray_dir.x;
		curr_pmap.y += ray_dir.y;
	}
}

static void	draw_filled_fov(t_cub *cub, t_2ipoint base_offset)
{
	float		player_angle;
	float		fov_half_angle;
	float		start_angle;
	float		end_angle;

	player_angle = atan2f(cub->raycaster.player.dir.y, \
		cub->raycaster.player.dir.x);
	fov_half_angle = FOV_RADIANS / 2.0f;
	start_angle = player_angle - fov_half_angle;
	end_angle = player_angle + fov_half_angle;
	while (start_angle <= end_angle)
	{
		cast_minimap_fov_ray(cub, base_offset, start_angle);
		start_angle += FOV_ANGLE_STEP;
	}
}

void	draw_player_n_fov(t_cub *cub, t_2ipoint screen_pos, int offset_from_x)
{
	screen_pos.x = offset_from_x + \
				(int)(cub->raycaster.player.pos.x * MINIMAP_CELL_SIZE);
	screen_pos.y = MINIMAP_OFFSET_Y + \
				(int)(cub->raycaster.player.pos.y * MINIMAP_CELL_SIZE);
	draw_minimap_square(&cub->pic.img, (t_2ipoint){screen_pos.x - \
		MINIMAP_PLAYER_SIZE / 2, screen_pos.y - MINIMAP_PLAYER_SIZE / 2}, \
						MINIMAP_PLAYER_SIZE, MINIMAP_PLAYER_COLOR);
	draw_filled_fov(cub, (t_2ipoint){offset_from_x, MINIMAP_OFFSET_Y});
}

void	draw_minimap(t_cub *cub)
{
	t_2ipoint		idx;
	t_2ipoint		screen_pos;
	unsigned int	cell_color;
	int				offset_from_x;

	offset_from_x = SCREEN_WIDTH - (cub->map.sizes.map_lenght \
		* MINIMAP_CELL_SIZE) - MINIMAP_RIGHT_MARGIN;
	idx.y = 0;
	while ((idx.y)++ < cub->map.sizes.map_height)
	{
		idx.x = 0;
		while ((idx.x)++ < cub->map.sizes.map_lenght)
		{
			screen_pos.x = offset_from_x + idx.x * MINIMAP_CELL_SIZE;
			screen_pos.y = MINIMAP_OFFSET_Y + idx.y * MINIMAP_CELL_SIZE;
			if (cub->map.matrix[idx.y * cub->map.sizes.map_lenght \
											+ idx.x] == WALL)
				cell_color = MINIMAP_WALL_COLOR;
			else
				cell_color = MINIMAP_FLOOR_COLOR;
			draw_minimap_square(&cub->pic.img, screen_pos, \
								MINIMAP_CELL_SIZE, cell_color);
		}
	}
	draw_player_n_fov(cub, screen_pos, offset_from_x);
}
