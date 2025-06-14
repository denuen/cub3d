/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_draws_utils_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 11:45:00 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/14 12:07:05 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

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

void	calculate_object_position(t_cub *cub, t_ray *ray, t_2ipoint tmp,
							t_2fpoint *pos)
{
	int		y_offset;
	float	third_distance;

	y_offset = tmp.y - SCREEN_HEIGHT / 2;
	if (y_offset == 0)
	{
		pos->x = cub->raycaster.player.pos.x;
		pos->y = cub->raycaster.player.pos.y;
		return ;
	}
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

t_image	*select_gun_texture(t_cub *cub, int type)
{
	if (type == NO_LIGHT)
		return (&cub->textures.gun_nofire);
	else
		return (&cub->textures.gun_fire);
}

void	draw_gun_pixel(t_cub *cub, t_image *gun, t_2ipoint screen, \
												t_2ipoint init_pos)
{
	t_2ipoint		tex;
	t_2ipoint		final_screen;
	unsigned int	color;
	char			*pixel_addr;
	int				scale;

	scale = 10;
	tex.y = screen.x / scale;
	tex.x = screen.y / scale;
	pixel_addr = gun->addr + tex.x * gun->lenght + tex.y * (gun->bits_pp / 8);
	color = *(unsigned int *)pixel_addr;
	if (!((color & 0x0FFFFFF) == 0xFF00FF || color == 0x0FFFFFF))
	{
		final_screen.x = init_pos.x + screen.x;
		final_screen.y = init_pos.y + screen.y;
		if (final_screen.x >= 0 && final_screen.x < SCREEN_WIDTH
			&& final_screen.y >= 0 && final_screen.y < SCREEN_HEIGHT)
			mymlx_pixel_put(&cub->pic.img, final_screen.x, \
				final_screen.y, color);
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
			mymlx_pixel_put(&cub->pic.img, x, y, filter(color, 0.5f));
		}
		y++;
	}
}
