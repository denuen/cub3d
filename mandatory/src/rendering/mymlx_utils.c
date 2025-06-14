/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mymlx_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 08:38:08 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/14 12:20:35 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include <stdlib.h>

void	load_texture(t_cub *cub, t_image *img, char *path)
{
	img->p = mlx_xpm_file_to_image(cub->p, path, &img->size.width, \
								&img->size.height);
	if (!img->p)
	{
		ft_printf("%s Error: cannot load xpm!\n", path);
		mymlx_exit(cub);
	}
	img->addr = mlx_get_data_addr(img->p, &img->bits_pp, \
								&img->lenght, &img->endian);
}

void	mymlx_init(t_cub *cub, char *argv[])
{
	if (!map_parsing(argv[1], &cub->map))
	{
		ft_printf("Error: Invalid map file\n");
		exit (EXIT_FAILURE);
	}
	cub->p = NULL;
	cub->pic.img.p = NULL;
	cub->pic.win.p = NULL;
	cub->p = mlx_init();
	if (!cub->p)
		exit(EXIT_FAILURE);
	cub->pic.win.p = mlx_new_window(cub->p, SCREEN_WIDTH, \
								SCREEN_HEIGHT, "Cub3d");
	cub->pic.img.p = mlx_new_image(cub->p, SCREEN_WIDTH, SCREEN_HEIGHT);
	cub->pic.img.addr = mlx_get_data_addr(cub->pic.img.p, \
			&cub->pic.img.bits_pp, &cub->pic.img.lenght, &cub->pic.img.endian);
	cub->pic.img.size.width = SCREEN_WIDTH;
	cub->pic.img.size.height = SCREEN_HEIGHT;
	load_texture(cub, &cub->textures.wall.north, cub->map.data.nt);
	load_texture(cub, &cub->textures.wall.south, cub->map.data.st);
	load_texture(cub, &cub->textures.wall.east, cub->map.data.et);
	load_texture(cub, &cub->textures.wall.west, cub->map.data.wt);
	cub->keys = (t_keys){0, 0, 0, 0};
	raycaster_init(cub);
	mlx_do_key_autorepeaton(cub->p);
}

int	mymlx_destroy(t_cub *cub)
{
	if (cub)
	{
		if (cub->pic.img.p)
			mlx_destroy_image(cub->p, cub->pic.img.p);
		if (cub->textures.wall.east.p)
			mlx_destroy_image(cub->p, cub->textures.wall.east.p);
		if (cub->textures.wall.west.p)
			mlx_destroy_image(cub->p, cub->textures.wall.west.p);
		if (cub->textures.wall.south.p)
			mlx_destroy_image(cub->p, cub->textures.wall.south.p);
		if (cub->textures.wall.north.p)
			mlx_destroy_image(cub->p, cub->textures.wall.north.p);
		if (cub->pic.win.p)
			mlx_destroy_window(cub->p, cub->pic.win.p);
		if (cub->raycaster.rays)
			free (cub->raycaster.rays);
		if (cub->p)
		{
			mlx_destroy_display(cub->p);
			free (cub->p);
		}
		return (1);
	}
	return (-1);
}

int	mymlx_exit(t_cub *cub)
{
	exit(mymlx_destroy(cub));
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
