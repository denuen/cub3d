/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mymlx_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 08:38:08 by apintaur          #+#    #+#             */
/*   Updated: 2025/07/25 14:57:36 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include <stdlib.h>

static void	load_textures(t_cub *cub);
void	safe_init(t_cub *cub);

void	mymlx_init(t_cub *cub, char *argv[])
{
	safe_init(cub);
	if (!map_parsing(argv[1], &cub->map))
	{
		ft_printf("Error: Invalid map file\n");
		exit (EXIT_FAILURE);
	}
	cub->p = mlx_init();
	cub->pic.win.p = mlx_new_window(cub->p, SCREEN_WIDTH,
			SCREEN_HEIGHT, "Cub3d");
	cub->pic.img.p = mlx_new_image(cub->p, SCREEN_WIDTH, SCREEN_HEIGHT);
	cub->pic.img.addr = mlx_get_data_addr(cub->pic.img.p,
			&cub->pic.img.bits_pp, &cub->pic.img.lenght, &cub->pic.img.endian);
	cub->pic.img.size = (t_size){SCREEN_WIDTH, SCREEN_HEIGHT};
	cub->mouse_times = 0;
	cub->mouse_x = 0;
	cub->gun_animation_frame = 0;
	cub->keys = (t_keys){0, 0, 0, 0, 0, 0};
	load_textures(cub);
	init_doors_state(&cub->map);
	raycaster_init(cub);
	init_fps_counter(cub);
	mlx_do_key_autorepeaton(cub->p);
}

void	update_dir(t_cub *cub, int type)
{
	float		old_dir;
	float		old_plane;
	t_2fpoint	*dir;
	t_2fpoint	*plane;

	old_dir = cub->raycaster.player.dir.x;
	old_plane = cub->raycaster.player.plane.x;
	dir = &cub->raycaster.player.dir;
	plane = &cub->raycaster.player.plane;
	if (type == LEFT)
	{
		dir->x = dir->x * cos(-ROT_SPD) - dir->y * sin(-ROT_SPD);
		dir->y = old_dir * sin(-ROT_SPD) + dir->y * cos(-ROT_SPD);
		plane->x = plane->x * cos(-ROT_SPD) - plane->y * sin(-ROT_SPD);
		plane->y = old_plane * sin(-ROT_SPD) + plane->y * cos(-ROT_SPD);
	}
	else if (type == RIGHT)
	{
		dir->x = dir->x * cos(ROT_SPD) - dir->y * sin(ROT_SPD);
		dir->y = old_dir * sin(ROT_SPD) + dir->y * cos(ROT_SPD);
		plane->x = plane->x * cos(ROT_SPD) - plane->y * sin(ROT_SPD);
		plane->y = old_plane * sin(ROT_SPD) + plane->y * cos(ROT_SPD);
	}
}

static void	load_single_texture(t_cub *cub, t_image *img, char *path)
{
	if (!path)
	{
		ft_printf("Error: NULL texture path\n");
		mymlx_exit(cub);
	}
	img->p = mlx_xpm_file_to_image(cub->p, path,
			&img->size.width, &img->size.height);
	if (!img->p)
	{
		ft_printf("%s Error: cannot load xpm!\n", path);
		mymlx_exit(cub);
	}
	img->addr = mlx_get_data_addr(img->p, &img->bits_pp,
			&img->lenght, &img->endian);
	if (!img->addr)
	{
		ft_printf("%s Error: cannot get texture data!\n", path);
		mymlx_exit(cub);
	}
}

static void	load_textures(t_cub *cub)
{
	load_single_texture(cub, &cub->textures.wall, cub->map.data.wall);
	load_single_texture(cub, &cub->textures.gun_fire, cub->map.data.gun1);
	load_single_texture(cub, &cub->textures.gun_nofire, cub->map.data.gun2);
	load_single_texture(cub, &cub->textures.door, cub->map.data.door);
	load_single_texture(cub, &cub->textures.ceiling_light,
		cub->map.data.ceiling1);
	load_single_texture(cub, &cub->textures.ceiling_nolight,
		cub->map.data.ceiling2);
	load_single_texture(cub, &cub->textures.hand, cub->map.data.hand);
	load_single_texture(cub, &cub->textures.floor_light, cub->map.data.floor1);
	load_single_texture(cub, &cub->textures.floor_nolight,
		cub->map.data.floor2);
}
