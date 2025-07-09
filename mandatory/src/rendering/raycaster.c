/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 08:38:14 by apintaur          #+#    #+#             */
/*   Updated: 2025/07/09 16:24:06 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	draw_line(t_image *img, int x,
			t_2ipoint y_interval, unsigned int color);
void	init_raydir_n_deltadist(t_ray *ray, t_player *p, float camera);
void	update_cast_data(t_ray *ray, t_player *p, float camera);
void	draw_textured_column(t_cub *cub, t_ray *ray, int x);

void	update_dda_step(t_ray *ray)
{
	int	is_x_shorter;

	is_x_shorter = (ray->side_dist.x < ray->side_dist.y);
	ray->side_dist.x += ray->delta_dist.x * (is_x_shorter);
	ray->side_dist.y += ray->delta_dist.y * (!is_x_shorter);
	ray->cell_pos.x += ray->step.x * (is_x_shorter);
	ray->cell_pos.y += ray->step.y * (!is_x_shorter);
	ray->side = !is_x_shorter;
}

static int	execute_dda_step(t_ray *ray, t_map *map)
{
	int		map_index;
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
	map_index = ray->cell_pos.y * map->sizes.map_lenght + ray->cell_pos.x;
	if (map->matrix[map_index] == WALL)
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

void	cast_ray(t_ray *ray, t_player *p, t_map *map, int x)
{
	float	camera;

	camera = 2 * x / (float)SCREEN_WIDTH - 1;
	update_cast_data(ray, p, camera);
	run_dda_algorithm(ray, map);
	if (ray->side == 0)
		ray->perp_wall_dist = fabsf((ray->cell_pos.x - p->pos.x
					+ (1 - ray->step.x) / 2) / ray->dir.x);
	else
		ray->perp_wall_dist = fabsf((ray->cell_pos.y - p->pos.y
					+ (1 - ray->step.y) / 2) / ray->dir.y);
	if (ray->perp_wall_dist <= 0.1f)
		ray->perp_wall_dist = 0.1f;
	ray->line_height = (int)(SCREEN_HEIGHT / ray->perp_wall_dist);
	ray->draw_start = (SCREEN_HEIGHT - ray->line_height) / 2;
	ray->draw_end = (SCREEN_HEIGHT + ray->line_height) / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	if (ray->draw_end >= SCREEN_HEIGHT)
		ray->draw_end = SCREEN_HEIGHT - 1;
}

void	render_column(t_cub *cub, int x,
		unsigned int ceiling_color, unsigned int floor_color)
{
	t_ray	*ray;
	int		i;
	int		inc;

	ray = &cub->raycaster.rays[x];
	draw_line(&cub->pic.img, x, (t_2ipoint){0, ray->draw_start - 1},
		ceiling_color);
	draw_textured_column(cub, ray, x);
	draw_line(&cub->pic.img, x, (t_2ipoint){ray->draw_end + 1,
		SCREEN_HEIGHT - 1}, floor_color);
	inc = (1 * (ray->perp_wall_dist > LOD_THRESHOLD))
		+ (RENDER_SCALE * (ray->perp_wall_dist > LOD_THRESHOLD));
	if (inc > 1 && x + inc < SCREEN_WIDTH)
	{
		i = 1;
		while (i < inc && x + i++ < SCREEN_WIDTH)
		{
			draw_line(&cub->pic.img, x + i,
				(t_2ipoint){0, ray->draw_start - 1}, ceiling_color);
			draw_textured_column(cub, ray, x + i);
			draw_line(&cub->pic.img, x + i,
				(t_2ipoint){ray->draw_end + 1, SCREEN_HEIGHT - 1}, floor_color);
		}
	}
}
