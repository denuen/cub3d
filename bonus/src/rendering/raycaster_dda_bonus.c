/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster_dda_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:12:14 by apintaur          #+#    #+#             */
/*   Updated: 2025/07/09 11:32:04 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	is_obstacle(t_ray *ray, t_map *map)
{
	int	door_state;

	if (map->matrix[ray->cell_pos.y * map->sizes.map_lenght
			+ ray->cell_pos.x] == WALL)
		return (1);
	if (map->matrix[ray->cell_pos.y * map->sizes.map_lenght
			+ ray->cell_pos.x] == DOOR)
	{
		door_state = map->doors_state[ray->cell_pos.y
			* map->sizes.map_lenght + ray->cell_pos.x];
		if (door_state == 0)
			return (1);
	}
	return (0);
}

static void	update_dda_step(t_ray *ray)
{
	if ((ray->side_dist.x < ray->side_dist.y))
	{
		ray->side_dist.x += ray->delta_dist.x;
		ray->cell_pos.x += ray->step.x;
		ray->side = 0;
	}
	else
	{
		ray->side_dist.y += ray->delta_dist.y;
		ray->cell_pos.y += ray->step.y;
		ray->side = 1;
	}
}

int	execute_dda_step(t_ray *ray, t_map *map)
{
	float	distance;

	update_dda_step(ray);
	if (ray->side == 0)
		distance = ray->side_dist.x - ray->delta_dist.x;
	else
		distance = ray->side_dist.y - ray->delta_dist.y;
	if (distance > VIEW_DISTANCE)
	{
		ray->perp_wall_dist = VIEW_DISTANCE;
		return (1);
	}
	if (ray->cell_pos.x < 0 || ray->cell_pos.x >= map->sizes.map_lenght
		|| ray->cell_pos.y < 0 || ray->cell_pos.y >= map->sizes.map_height)
	{
		ray->perp_wall_dist = VIEW_DISTANCE;
		return (1);
	}
	if (is_obstacle(ray, map))
		return (1);
	return (0);
}

void	run_dda_algorithm(t_ray *ray, t_map *map)
{
	int	count;

	count = 0;
	while (count < (int)(VIEW_DISTANCE * 2.0f))
	{
		if (execute_dda_step(ray, map))
			return ;
		count++;
	}
	ray->perp_wall_dist = VIEW_DISTANCE;
}
