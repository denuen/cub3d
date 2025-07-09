/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 12:46:09 by apintaur          #+#    #+#             */
/*   Updated: 2025/07/09 11:35:28 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	cast_minimap_fov_ray(t_cub *cub, t_2ipoint base_offset, float angle)
{
	t_2fpoint	ray_dir;
	t_2fpoint	pc_pos;
	t_2ipoint	check;
	t_2ipoint	utils;

	utils = (t_2ipoint){SCREEN_WIDTH / MINIMAP_SCALE_FACTOR, 0};
	ray_dir = (t_2fpoint){cosf(angle) * 0.03f, sinf(angle) * 0.03f};
	pc_pos = cub->raycaster.player.pos;
	while ((utils.y)++ < (utils.x) * 5)
	{
		if ((int)pc_pos.x < 0 || (int)pc_pos.x >= cub->map.sizes.map_lenght
			|| (int)pc_pos.y < 0 || (int)pc_pos.y >= cub->map.sizes.map_height)
			break ;
		check.x = base_offset.x + (int)(pc_pos.x * (utils.x));
		check.y = base_offset.y + (int)(pc_pos.y * (utils.x));
		if ((check.x >= 0 && check.x < SCREEN_WIDTH)
			&& (check.y >= 0 && check.y < SCREEN_HEIGHT))
			mymlx_pixel_put(&cub->pic.img, check.x, check.y, FOV_FILL_COLOR);
		pc_pos.x += ray_dir.x;
		pc_pos.y += ray_dir.y;
	}
}

void	draw_filled_fov(t_cub *cub, t_2ipoint base_offset)
{
	float		player_angle;
	float		fov_half_angle;
	float		start_angle;
	float		end_angle;

	player_angle = atan2f(cub->raycaster.player.dir.y,
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

void	draw_minimap_square(t_image *img, t_2ipoint start,
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
			if ((start.x + x >= 0 && start.x + x < SCREEN_WIDTH)
				&& (start.y + y >= 0 && start.y + y < SCREEN_HEIGHT))
				mymlx_pixel_put(img, start.x + x, start.y + y, color);
			x++;
		}
		y++;
	}
}

void	draw_minimap_cell(t_cub *cub, t_2ipoint idx, int cell_size,
	int offset_from_x)
{
	t_2ipoint		screen_pos;
	unsigned int	cell_color;
	int				matrix_index;

	screen_pos.x = offset_from_x + idx.x * cell_size;
	screen_pos.y = (SCREEN_HEIGHT / 60) + idx.y * cell_size;
	matrix_index = idx.y * cub->map.sizes.map_lenght + idx.x;
	if (matrix_index >= 0 && matrix_index < (cub->map.sizes.map_height
			* cub->map.sizes.map_lenght))
	{
		if (cub->map.matrix[matrix_index] == WALL)
			cell_color = MINIMAP_WALL_COLOR;
		else
			cell_color = MINIMAP_FLOOR_COLOR;
		draw_minimap_square(&cub->pic.img, screen_pos, cell_size, cell_color);
	}
}
