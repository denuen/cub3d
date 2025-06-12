/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_system_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:11:03 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 19:22:33 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	init_doors_state(t_map *map)
{
	int	total_cells;
	int	i;

	total_cells = map->sizes.map_height * map->sizes.map_lenght;
	map->doors_state = ft_calloc(total_cells, sizeof(int));
	if (!map->doors_state)
	{
		ft_printf("Error: Calloc failed in doors state's initialization\n");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < total_cells)
	{
		map->doors_state[i] = 0;
		i++;
	}
}

int	is_door_open(t_map *map, int x, int y)
{
	int	idx;

	if (x < 0 || x >= map->sizes.map_lenght \
		|| y < 0 || y >= map->sizes.map_height)
		return (0);
	idx = (y * map->sizes.map_lenght + x);
	if (idx < 0 || idx >= (map->sizes.map_height * map->sizes.map_lenght))
		return (0);
	return (map->doors_state[idx] == 1);
}

static int	is_player_near_door(t_cub *cub, int door_x, int door_y)
{
	float	player_x;
	float	player_y;
	float	distance;
	float	interaction_range;

	player_x = cub->raycaster.player.pos.x;
	player_y = cub->raycaster.player.pos.y;
	interaction_range = 1.5f;
	distance = sqrtf((player_x - door_x) * (player_x - door_x) \
					+ (player_y - door_y) * (player_y - door_y));
	return (distance <= interaction_range);
}

void	toggle_door(t_cub *cub)
{
	t_2ipoint	tmp;
	int			idx;
	int			total_cells;

	total_cells = cub->map.sizes.map_height * cub->map.sizes.map_lenght;
	tmp.y = 0;
	while ((tmp.y)++ < cub->map.sizes.map_height)
	{
		tmp.x = 0;
		while ((tmp.x)++ < cub->map.sizes.map_lenght)
		{
			idx = (tmp.y * cub->map.sizes.map_lenght + tmp.x);
			if (idx >= 0 && idx < total_cells
				&& cub->map.matrix[idx] == DOOR
				&& is_player_near_door(cub, tmp.x, tmp.y))
			{
				if (cub->map.doors_state[idx] == 0)
					cub->map.doors_state[idx] = 1;
				else
					cub->map.doors_state[idx] = 0;
				return ;
			}
		}
	}
}
