/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 08:38:11 by apintaur          #+#    #+#             */
/*   Updated: 2025/07/09 16:22:55 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	is_player(char c);

static void	init_dir_n_plane(char type, t_player *player)
{
	if (type == 'N')
	{
		player->dir = (t_2fpoint){0.0f, -1.0f};
		player->plane = (t_2fpoint){0.66f, 0.0f};
	}
	else if (type == 'S')
	{
		player->dir = (t_2fpoint){0.0f, 1.0f};
		player->plane = (t_2fpoint){-0.66f, 0.0f};
	}
	else if (type == 'E')
	{
		player->dir = (t_2fpoint){1.0f, 0.0f};
		player->plane = (t_2fpoint){0.0f, 0.66f};
	}
	else if (type == 'W')
	{
		player->dir = (t_2fpoint){-1.0f, 0.0f};
		player->plane = (t_2fpoint){0.0f, -0.66f};
	}
}

void	raycaster_init(t_cub *cub)
{
	t_2ipoint	idx;
	t_sizes		size;
	char		player_char;

	cub->raycaster.rays = (t_ray *) malloc(sizeof(t_ray) * SCREEN_WIDTH);
	size = cub->map.sizes;
	idx = (t_2ipoint){0, 0};
	while ((idx.y)++ < size.map_height)
	{
		idx.x = 0;
		while ((idx.x)++ < size.map_lenght)
		{
			if (is_player(cub->map.matrix[idx.y * size.map_lenght + idx.x]))
			{
				player_char = (char)cub->map.matrix[idx.y
					* size.map_lenght + idx.x];
				cub->raycaster.player.pos.x = idx.x + 0.5f;
				cub->raycaster.player.pos.y = idx.y + 0.5f;
				init_dir_n_plane(player_char, &cub->raycaster.player);
				cub->map.matrix[idx.y * size.map_lenght + idx.x] = FLOOR;
				return ;
			}
		}
	}
}

static void	init_side_dist(t_ray *ray, t_player *p)
{
	t_2fpoint	pos_diff;

	if (ray->dir.x < 0)
	{
		ray->step.x = -1;
		pos_diff.x = p->pos.x - ray->cell_pos.x;
	}
	else
	{
		ray->step.x = 1;
		pos_diff.x = ray->cell_pos.x + 1.0f - p->pos.x;
	}
	if (ray->dir.y < 0)
	{
		ray->step.y = -1;
		pos_diff.y = p->pos.y - ray->cell_pos.y;
	}
	else
	{
		ray->step.y = 1;
		pos_diff.y = ray->cell_pos.y + 1.0f - p->pos.y;
	}
	ray->side_dist.x = pos_diff.x * ray->delta_dist.x;
	ray->side_dist.y = pos_diff.y * ray->delta_dist.y;
}

static void	init_raydir_n_deltadist(t_ray *ray, t_player *p, float camera)
{
	ray->dir.x = p->dir.x + p->plane.x * camera;
	ray->dir.y = p->dir.y + p->plane.y * camera;
	ray->cell_pos.x = (int)p->pos.x;
	ray->cell_pos.y = (int)p->pos.y;
	if (fabsf(ray->dir.x) < 0.00001f)
		ray->delta_dist.x = 1e30;
	else
		ray->delta_dist.x = fabsf(1.0f / ray->dir.x);
	if (fabsf(ray->dir.y) < 0.00001f)
		ray->delta_dist.y = 1e30;
	else
		ray->delta_dist.y = fabsf(1.0f / ray->dir.y);
}

void	update_cast_data(t_ray *ray, t_player *p, float camera)
{
	init_raydir_n_deltadist(ray, p, camera);
	init_side_dist(ray, p);
}
