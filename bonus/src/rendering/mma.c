/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mma.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:39:59 by apintaur          #+#    #+#             */
/*   Updated: 2025/07/25 15:25:20 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include <stdlib.h>

void	safe_init(t_cub *cub)
{
	cub->p = NULL;
	cub->pic.img.p = NULL;
	cub->pic.win.p = NULL;
	cub->map.data.wall = NULL;
	cub->map.data.door = NULL;
	cub->map.data.gun1 = NULL;
	cub->map.data.gun2 = NULL;
	cub->map.data.ceiling1 = NULL;
	cub->map.data.ceiling2 = NULL;
	cub->map.data.ceiling3 = NULL;
	cub->map.data.hand = NULL;
	cub->map.data.floor1 = NULL;
	cub->map.data.floor2 = NULL;
	cub->map.data.floor3 = NULL;
	cub->map.data.floor4 = NULL;
	cub->map.data.floor5 = NULL;
	cub->map.data.floor6 = NULL;
	cub->map.data.floor7 = NULL;
	cub->map.data.floor8 = NULL;
	cub->map.matrix = NULL;
	cub->raycaster.rays = NULL;
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
