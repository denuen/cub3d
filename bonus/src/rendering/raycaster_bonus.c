/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 08:38:14 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 19:56:09 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	calculate_wall_height(t_ray *ray);
static void	calculate_perp_wall_dist(t_ray *ray, t_player *p);
void		init_delta_dist(t_ray *ray);
void		init_side_dist(t_ray *ray, t_player *p);
void		init_ray_dir(t_ray *ray, t_player *p, float camera);

void	cast_ray(t_ray *ray, t_player *p, t_map *map, int x)
{
	float	camera;

	camera = 2 * x / (float)SCREEN_WIDTH - 1;
	init_ray_dir(ray, p, camera);
	init_delta_dist(ray);
	init_side_dist(ray, p);
	run_dda_algorithm(ray, map);
	calculate_perp_wall_dist(ray, p);
	calculate_wall_height(ray);
}

static void	calculate_wall_height(t_ray *ray)
{
	ray->line_height = (int)(SCREEN_HEIGHT / ray->perp_wall_dist);
	ray->draw_start = (SCREEN_HEIGHT - ray->line_height) / 2;
	ray->draw_end = (SCREEN_HEIGHT + ray->line_height) / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	if (ray->draw_end >= SCREEN_HEIGHT)
		ray->draw_end = SCREEN_HEIGHT - 1;
}

static void	calculate_perp_wall_dist(t_ray *ray, t_player *p)
{
	if (ray->side == 0)
	{
		ray->perp_wall_dist = fabsf((ray->cell_pos.x - p->pos.x
					+ (1 - ray->step.x) / 2) / ray->dir.x);
	}
	else
	{
		ray->perp_wall_dist = fabsf((ray->cell_pos.y - p->pos.y
					+ (1 - ray->step.y) / 2) / ray->dir.y);
	}
	if (ray->perp_wall_dist <= 0.1f)
		ray->perp_wall_dist = 0.1f;
}

void	set_gun_state(t_cub *cub)
{
	if (cub->keys.space)
	{
		cub->gun_animation_frame++;
		if ((cub->gun_animation_frame / 4) % 2 == 0)
			draw_gun(cub, LIGHT);
		else
			draw_gun(cub, NO_LIGHT);
	}
	else
	{
		cub->gun_animation_frame = 0;
		draw_gun(cub, NO_LIGHT);
	}
}
