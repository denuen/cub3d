/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_draws.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:52:46 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 19:07:51 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_line(t_image *img, int x, t_2ipoint y_interval, unsigned int color)
{
	int		y;
	char	*dest;

	if (x < 0 || x >= SCREEN_WIDTH)
		return ;
	if (y_interval.x < 0)
		y_interval.x = 0;
	if (y_interval.y >= SCREEN_HEIGHT)
		y_interval.y = SCREEN_HEIGHT - 1;
	if (y_interval.x > y_interval.y)
		return ;
	dest = img->addr + (y_interval.x * img->lenght + x * (img->bits_pp / 8));
	y = y_interval.x;
	while (y <= y_interval.y)
	{
		*((unsigned int *)dest) = color;
		dest += img->lenght;
		y++;
	}
}

static t_image	*select_wall_texture(t_cub *cub, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->step.x > 0)
			return (&cub->textures.wall.east);
		else
			return (&cub->textures.wall.west);
	}
	else
	{
		if (ray->step.y > 0)
			return (&cub->textures.wall.south);
		else
			return (&cub->textures.wall.north);
	}
}

static double	calculate_wall_x(t_cub *cub, t_ray *ray)
{
	double	wall_x;

	if (ray->side == 0)
		wall_x = cub->raycaster.player.pos.y
			+ ray->perp_wall_dist * ray->dir.y;
	else
		wall_x = cub->raycaster.player.pos.x
			+ ray->perp_wall_dist * ray->dir.x;
	wall_x -= floor(wall_x);
	return (wall_x);
}

static void	draw_texture_pixel(t_cub *cub, t_image *texture, t_2ipoint tex_pos,
						t_2ipoint point)
{
	char			*pixel;
	unsigned int	color;

	if (tex_pos.y < 0 || tex_pos.y >= texture->size.height
		|| tex_pos.x < 0 || tex_pos.x >= texture->size.width)
		return ;
	pixel = texture->addr + (tex_pos.y * texture->lenght
			+ tex_pos.x * (texture->bits_pp / 8));
	color = *(unsigned int *)pixel;
	mymlx_pixel_put(&cub->pic.img, point.x, point.y, color);
}

void	draw_textured_column(t_cub *cub, t_ray *ray, int x)
{
	t_image			*texture;
	double			wall_x;
	t_2ipoint		tex_pos;
	t_2ipoint		tmp;

	if (!cub || !ray)
		return ;
	texture = select_wall_texture(cub, ray);
	if (!texture || !texture->addr)
		return ;
	wall_x = calculate_wall_x(cub, ray);
	tex_pos.x = (int)(wall_x * texture->size.width);
	if ((ray->side == 0 && ray->dir.x > 0)
		|| (ray->side == 1 && ray->dir.y < 0))
		tex_pos.x = texture->size.width - tex_pos.x - 1;
	tmp.y = ray->draw_start;
	while (tmp.y <= ray->draw_end)
	{
		tmp.x = tmp.y * 256 - SCREEN_HEIGHT * 128 + ray->line_height * 128;
		tex_pos.y = (tmp.x * texture->size.height) / ray->line_height / 256;
		draw_texture_pixel(cub, texture, tex_pos, (t_2ipoint){x, tmp.y});
		(tmp.y)++;
	}
}
