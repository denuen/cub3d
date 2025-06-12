/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mymlx_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:00:06 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 19:56:15 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	set_gun_state(t_cub *cub);

int	mymlx_render(t_cub *cub)
{
	int				x;

	ft_bzero(cub->pic.img.addr,
		SCREEN_WIDTH * SCREEN_HEIGHT * (cub->pic.img.bits_pp / 8));
	x = 0;
	while (x < SCREEN_WIDTH)
	{
		cast_ray(&cub->raycaster.rays[x],
			&cub->raycaster.player, &cub->map, x);
		render_textures(cub, x, &cub->raycaster.rays[x]);
		if (cub->raycaster.rays[x].perp_wall_dist > LOD_THRESHOLD)
			x += RENDER_SCALE;
		else
			x++;
	}
	draw_minimap(cub);
	set_gun_state(cub);
	mlx_put_image_to_window(cub->p, cub->pic.win.p,
		cub->pic.img.p, 0, 0);
	update_fps_counter(cub);
	return (0);
}

void	mymlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*tmp_addr;

	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
	{
		tmp_addr = img->addr + (y * img->lenght \
			+ x * (img->bits_pp / 8));
		*((unsigned int *)tmp_addr) = color;
	}
}

int	mymlx_destroy(t_cub *cub)
{
	if (cub)
	{
		mlx_destroy_image(cub->p, cub->pic.img.p);
		mlx_destroy_image(cub->p, cub->textures.wall.p);
		mlx_destroy_image(cub->p, cub->textures.door.p);
		mlx_destroy_image(cub->p, cub->textures.gun_fire.p);
		mlx_destroy_image(cub->p, cub->textures.gun_nofire.p);
		mlx_destroy_image(cub->p, cub->textures.ceiling_light.p);
		mlx_destroy_image(cub->p, cub->textures.ceiling_nolight.p);
		mlx_destroy_image(cub->p, cub->textures.floor_light.p);
		mlx_destroy_image(cub->p, cub->textures.floor_nolight.p);
		mlx_destroy_window(cub->p, cub->pic.win.p);
		free (cub->raycaster.rays);
		if (cub->map.doors_state)
			free(cub->map.doors_state);
		free (cub->p);
		return (1);
	}
	return (-1);
}

int	mymlx_exit(t_cub *cub)
{
	exit(mymlx_destroy(cub));
}
