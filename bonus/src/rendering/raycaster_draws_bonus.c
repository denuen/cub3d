/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_draws_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 08:38:11 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 19:32:40 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_player(char c);

unsigned int	filter(unsigned int color, float factor)
{
	unsigned char	red;
	unsigned char	green;
	unsigned char	blue;

	red = ((color >> 16) & 0xFF) * factor;
	green = ((color >> 8) & 0xFF) * factor;
	blue = (color & 0xFF) * factor;
	return (((unsigned int)red << 16) | ((unsigned int)green << 8) | blue);
}

static void	calculate_object_position(t_cub *cub, t_ray *ray, t_2ipoint tmp,
							t_2fpoint *pos)
{
	int		y_offset;
	float	third_distance;

	y_offset = tmp.y - SCREEN_HEIGHT / 2;
	third_distance = (0.5 * SCREEN_HEIGHT) / y_offset;
	if (tmp.x == FLOOR)
	{
		pos->x = cub->raycaster.player.pos.x + third_distance * ray->dir.x;
		pos->y = cub->raycaster.player.pos.y + third_distance * ray->dir.y;
	}
	else if (tmp.x == CEILING)
	{
		pos->x = cub->raycaster.player.pos.x - third_distance * ray->dir.x;
		pos->y = cub->raycaster.player.pos.y - third_distance * ray->dir.y;
	}
}

static t_image	*select_gun_texture(t_cub *cub, int type)
{
	if (type == NO_LIGHT)
		return (&cub->textures.gun_nofire);
	else
		return (&cub->textures.gun_fire);
}

static void	draw_gun_pixel(t_cub *cub, t_image *gun, t_2ipoint screen,
						t_2ipoint init_pos, int scale)
{
	t_2ipoint		tex;
	t_2ipoint		final_screen;
	unsigned int	color;
	char			*pixel_addr;

	tex.y = screen.x / scale;
	tex.x = screen.y / scale;
	pixel_addr = gun->addr + tex.x * gun->lenght + tex.y * (gun->bits_pp / 8);
	color = *(unsigned int *)pixel_addr;
	if (!((color & 0x0FFFFFF) == 0xFF00FF || color == 0x0FFFFFF))
	{
		final_screen.x = init_pos.x + screen.x;
		final_screen.y = init_pos.y + screen.y;
		if (final_screen.x >= 0 && final_screen.x < SCREEN_WIDTH \
			&& final_screen.y >= 0 && final_screen.y < SCREEN_HEIGHT)
			mymlx_pixel_put(&cub->pic.img, final_screen.x,
				final_screen.y, color);
	}
}

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
			draw_gun_pixel(cub, gun, screen, init_pos, scale);
			screen.x++;
		}
		screen.y++;
	}
}

void	draw_ceiling(t_cub *cub, t_ray *ray, t_image *texture, int x)
{
	int				y;
	t_2ipoint		tex_pos;
	t_2fpoint		ceiling_pos;
	unsigned int	color;

	y = 0;
	while (y < ray->draw_start)
	{
		calculate_object_position(cub, ray, (t_2ipoint){CEILING, y},
			&ceiling_pos);
		tex_pos.x = (int)(ceiling_pos.x * CELL_SIZE) % texture->size.width;
		tex_pos.y = (int)(ceiling_pos.y * CELL_SIZE) % texture->size.height;
		if (tex_pos.y >= 0 && tex_pos.y < texture->size.height
			&& tex_pos.x >= 0 && tex_pos.x < texture->size.width)
		{
			color = *(unsigned int *)(texture->addr \
				+ (tex_pos.y * texture->lenght \
				+ tex_pos.x * (texture->bits_pp / 8)));
			mymlx_pixel_put(&cub->pic.img, x, y, filter(color, 0.4f));
		}
		y++;
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
			color = *(unsigned int *)(texture->addr \
				+ (tex_pos.y * texture->lenght \
				+ tex_pos.x * (texture->bits_pp / 8)));
			mymlx_pixel_put(&cub->pic.img, x, y, filter(color, 0.6f));
		}
		y++;
	}
}

void	draw_wall(t_cub *cub, t_ray *ray, t_image *tx, int x)
{
	int				y;
	double			wall_x;
	t_2ipoint		tx_pos;
	double			step;
	double			tex_pos;
	unsigned int	color;

	if (ray->side == 0)
		wall_x = cub->raycaster.player.pos.y
			+ ray->perp_wall_dist * ray->dir.y;
	else
		wall_x = cub->raycaster.player.pos.x
			+ ray->perp_wall_dist * ray->dir.x;
	wall_x -= floor(wall_x);
	tx_pos.x = (int)(wall_x * tx->size.width);
	if ((ray->side == 0 && ray->dir.x > 0)
		|| (ray->side == 1 && ray->dir.y < 0))
		tx_pos.x = tx->size.width - tx_pos.x - 1;
	step = (double)tx->size.height / ray->line_height;
	tex_pos = (ray->draw_start - SCREEN_HEIGHT / 2 \
		+ ray->line_height / 2) * step;
	if (tex_pos < 0)
		tex_pos = 0;
	y = ray->draw_start;
	if (y < 0)
		y = 0;
	while (y <= ray->draw_end && y < SCREEN_HEIGHT)
	{
		tx_pos.y = (int)tex_pos & (tx->size.height - 1);
		tex_pos += step;
		color = *(unsigned int *)(tx->addr \
			+ (tx_pos.y * tx->lenght \
			+ tx_pos.x * (tx->bits_pp / 8)));
		mymlx_pixel_put(&cub->pic.img, x, y, filter(color, 0.6f));
		y++;
	}
}

void	render_textures(t_cub *cub, int x, t_ray *ray)
{
	t_image	*texture;
	int		cell_value;

	if (x < 0 || x >= SCREEN_WIDTH)
		return ;
	texture = &cub->textures.ceiling_nolight;
	if (texture && texture->addr && texture->p)
		draw_ceiling(cub, ray, texture, x);
	cell_value = cub->map.matrix[ray->cell_pos.y * cub->map.sizes.map_lenght
		+ ray->cell_pos.x];
	if (cell_value == DOOR)
	{
		if (cub->map.doors_state[ray->cell_pos.y * cub->map.sizes.map_lenght \
			+ ray->cell_pos.x] == 0)
		{
			texture = &cub->textures.door;
			if (texture && texture->addr && texture->p)
				draw_wall(cub, ray, texture, x);
		}
	}
	else
	{
		texture = &cub->textures.wall;
		if (texture && texture->addr && texture->p)
			draw_wall(cub, ray, texture, x);
	}
	texture = &cub->textures.floor_nolight;
	if (texture && texture->addr && texture->p)
		draw_floor(cub, ray, texture, x);
}
