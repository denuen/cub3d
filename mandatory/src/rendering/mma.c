/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mma.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:39:59 by apintaur          #+#    #+#             */
/*   Updated: 2025/07/25 15:16:01 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include <stdlib.h>

void	safe_init(t_cub *cub)
{
	cub->p = NULL;
	cub->pic.img.p = NULL;
	cub->pic.win.p = NULL;
	cub->map.data.nt = NULL;
	cub->map.data.st = NULL;
	cub->map.data.et = NULL;
	cub->map.data.wt = NULL;
	cub->map.matrix = NULL;
	cub->raycaster.rays = NULL;
	cub->textures.wall.north.p = NULL;
	cub->textures.wall.south.p = NULL;
	cub->textures.wall.west.p = NULL;
	cub->textures.wall.east.p = NULL;
}

void	safe_free(void **p)
{
	if (p && *p)
		free (*p);
	*p = NULL;
}

void	safe_destroy_img(void *mlx, void **img_p)
{
	if (mlx && img_p && *img_p)
		mlx_destroy_image(mlx, *img_p);
}
