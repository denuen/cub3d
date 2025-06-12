/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mymlx.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 08:38:01 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 19:13:23 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	check_collisions(t_cub *cub, t_2fpoint new_pos, t_2fpoint old_pos);
void	update_dir(t_cub *cub, int type);

int	key_press(int keycode, t_cub *cub)
{
	if (keycode == XK_w)
		cub->keys.w = 1;
	else if (keycode == XK_s)
		cub->keys.s = 1;
	else if (keycode == XK_a)
	{
		cub->keys.a = 1;
	}
	else if (keycode == XK_d)
	{
		cub->keys.d = 1;
	}
	else if (keycode == XK_Escape)
		exit (mymlx_exit(cub));
	return (1);
}

int	key_release(int keycode, t_cub *cub)
{
	if (keycode == XK_w)
		cub->keys.w = 0;
	else if (keycode == XK_s)
		cub->keys.s = 0;
	else if (keycode == XK_a)
	{
		cub->keys.a = 0;
	}
	else if (keycode == XK_d)
	{
		cub->keys.d = 0;
	}
	return (1);
}

int	key_handler(t_cub *cub)
{
	t_2fpoint		new_pos;

	new_pos = cub->raycaster.player.pos;
	if (cub->keys.w)
	{
		new_pos.x += cub->raycaster.player.dir.x * MOVE_SPEED;
		new_pos.y += cub->raycaster.player.dir.y * MOVE_SPEED;
	}
	if (cub->keys.s)
	{
		new_pos.x -= cub->raycaster.player.dir.x * MOVE_SPEED;
		new_pos.y -= cub->raycaster.player.dir.y * MOVE_SPEED;
	}
	if (cub->keys.a)
		update_dir(cub, LEFT);
	if (cub->keys.d)
		update_dir(cub, RIGHT);
	check_collisions(cub, new_pos, cub->raycaster.player.pos);
	return (mymlx_render(cub));
}

int	mymlx_render(t_cub *cub)
{
	unsigned int	ceiling_color;
	unsigned int	floor_color;
	int				x;

	ceiling_color = cub->map.data.ceiling;
	floor_color = cub->map.data.floor;
	ft_bzero(cub->pic.img.addr,
		SCREEN_WIDTH * SCREEN_HEIGHT * (cub->pic.img.bits_pp / 8));
	x = 0;
	while (x < SCREEN_WIDTH)
	{
		cast_ray(&cub->raycaster.rays[x],
			&cub->raycaster.player, &cub->map, x);
		render_column(cub, x, ceiling_color, floor_color);
		if (cub->raycaster.rays[x].perp_wall_dist > LOD_THRESHOLD)
			x += RENDER_SCALE;
		else
			x += 1;
	}
	mlx_put_image_to_window(cub->p, cub->pic.win.p,
		cub->pic.img.p, 0, 0);
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
