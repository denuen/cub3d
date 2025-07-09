/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_draws_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 08:38:11 by apintaur          #+#    #+#             */
/*   Updated: 2025/07/09 11:25:20 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_player(char c);

void	draw_gun(t_cub *cub, int type)
{
	t_image		*gun;
	int			scale;
	t_2ipoint	init_pos;
	t_2ipoint	screen;

	gun = select_gun_texture(cub, type);
	if (!gun || !gun->addr)
		return ;
	scale = 10;
	init_pos.x = (SCREEN_WIDTH - gun->size.width * scale) / 2;
	init_pos.y = SCREEN_HEIGHT - gun->size.height * scale;
	screen.y = 0;
	while (screen.y < gun->size.height * scale)
	{
		screen.x = 0;
		while (screen.x < gun->size.width * scale)
		{
			draw_gun_pixel(cub, gun, screen, init_pos);
			screen.x++;
		}
		screen.y++;
	}
}

void	draw_floor(t_cub *cub, t_ray *ray, t_image *texture, int x)
{
	int				y;
	t_2ipoint		tex_pos;
	t_2fpoint		floor_pos;
	unsigned int	color;

	y = ray->draw_end + 1;
	while (y < SCREEN_HEIGHT)
	{
		calculate_object_position(cub, ray, (t_2ipoint){FLOOR, y}, &floor_pos);
		tex_pos.x = (int)(floor_pos.x * CELL_SIZE) % texture->size.width;
		tex_pos.y = (int)(floor_pos.y * CELL_SIZE) % texture->size.height;
		if (tex_pos.y >= 0 && tex_pos.y < texture->size.height
			&& tex_pos.x >= 0 && tex_pos.x < texture->size.width)
		{
			color = *(unsigned int *)(texture->addr
					+ (tex_pos.y * texture->lenght
						+ tex_pos.x * (texture->bits_pp / 8)));
			mymlx_pixel_put(&cub->pic.img, x, y, filter(color, 0.6f));
		}
		y++;
	}
}

static t_2ipoint	get_wall_texture_pos(t_cub *cub, t_ray *ray,
	t_image *texture)
{
	t_2fpoint	wall_pos;
	t_2ipoint	tex_coord;

	if (ray->side == 0)
		wall_pos.x = cub->raycaster.player.pos.y + ray->perp_wall_dist
			* ray->dir.y;
	else
		wall_pos.x = cub->raycaster.player.pos.x + ray->perp_wall_dist
			* ray->dir.x;
	wall_pos.x -= floor(wall_pos.x);
	tex_coord.x = (int)(wall_pos.x * texture->size.width);
	if ((ray->side == 0 && ray->dir.x > 0)
		|| (ray->side == 1 && ray->dir.y < 0))
		tex_coord.x = texture->size.width - tex_coord.x - 1;
	return (tex_coord);
}

void	draw_wall(t_cub *cub, t_ray *ray, t_image *tx, int x)
{
	t_2ipoint		tex_pos;
	t_2fpoint		tex_calc;
	unsigned int	color;
	int				y;

	tex_pos = get_wall_texture_pos(cub, ray, tx);
	if (ray->line_height <= 0)
		return ;
	tex_calc.y = (double)tx->size.height / ray->line_height;
	tex_calc.x = (ray->draw_start - SCREEN_HEIGHT / 2
			+ ray->line_height / 2) * tex_calc.y;
	y = (ray->draw_start) * (ray->draw_start > 0);
	while (y <= ray->draw_end && y < SCREEN_HEIGHT)
	{
		tex_pos.y = (int)tex_calc.x & (tx->size.height - 1);
		if (tex_pos.y >= 0 && tex_pos.y < tx->size.height
			&& tex_pos.x >= 0 && tex_pos.x < tx->size.width)
		{
			color = *(unsigned int *)(tx->addr + tex_pos.y * tx->lenght
					+ tex_pos.x * (tx->bits_pp / 8));
			mymlx_pixel_put(&cub->pic.img, x, y, filter(color, 0.6f));
		}
		tex_calc.x += tex_calc.y;
		y++;
	}
}

void	render_textures(t_cub *cub, int x, t_ray *ray)
{
	int		cell_value;

	if (x < 0 || x >= SCREEN_WIDTH)
		return ;
	if (cub->textures.ceiling_nolight.addr && cub->textures.ceiling_nolight.p)
		draw_ceiling(cub, ray, &cub->textures.ceiling_nolight, x);
	cell_value = cub->map.matrix[ray->cell_pos.y * cub->map.sizes.map_lenght
		+ ray->cell_pos.x];
	if (cell_value == DOOR)
	{
		if (cub->map.doors_state[ray->cell_pos.y * cub->map.sizes.map_lenght
				+ ray->cell_pos.x] == 0)
		{
			if (cub->textures.door.addr && cub->textures.door.p)
				draw_wall(cub, ray, &cub->textures.door, x);
		}
	}
	else
	{
		if (cub->textures.wall.addr && cub->textures.wall.p)
			draw_wall(cub, ray, &cub->textures.wall, x);
	}
	if (cub->textures.floor_nolight.addr && cub->textures.floor_nolight.p)
		draw_floor(cub, ray, &cub->textures.floor_nolight, x);
}
