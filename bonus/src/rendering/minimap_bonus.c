/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 08:21:02 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/14 12:52:02 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"
#include <math.h>

void	draw_filled_fov(t_cub *cub, t_2ipoint base_offset);
void	draw_minimap_cell(t_cub *cub, t_2ipoint idx, int cell_size, \
								int offset_from_x);
void	draw_minimap_square(t_image *img, t_2ipoint start, \
				int size, unsigned int color);

void	draw_player_n_fov(t_cub *cub, int offset_from_x)
{
	int			cell_size;
	int			player_size;
	t_2ipoint	screen_pos;

	cell_size = SCREEN_WIDTH / MINIMAP_SCALE_FACTOR;
	player_size = cell_size / 2;
	screen_pos = (t_2ipoint){0, 0};
	screen_pos.x = offset_from_x + (int)(cub->raycaster.player.pos.x
			* SCREEN_WIDTH / MINIMAP_SCALE_FACTOR);
	screen_pos.y = (SCREEN_HEIGHT / 60) + (int)(cub->raycaster.player.pos.y
			* SCREEN_WIDTH / MINIMAP_SCALE_FACTOR);
	draw_minimap_square(&cub->pic.img, (t_2ipoint){screen_pos.x - player_size, \
			screen_pos.y - player_size}, cell_size, MINIMAP_PLAYER_COLOR);
	draw_filled_fov(cub, (t_2ipoint){offset_from_x, (SCREEN_HEIGHT / 60)});
}

static void	draw_minimap_grid(t_cub *cub, int cell_size, int offset_from_x)
{
	t_2ipoint	idx;

	idx.y = 0;
	while (idx.y < cub->map.sizes.map_height)
	{
		idx.x = 0;
		while (idx.x < cub->map.sizes.map_lenght)
		{
			draw_minimap_cell(cub, idx, cell_size, offset_from_x);
			idx.x++;
		}
		idx.y++;
	}
}

void	draw_minimap(t_cub *cub)
{
	int	cell_size;
	int	offset_from_x;

	cell_size = SCREEN_WIDTH / MINIMAP_SCALE_FACTOR;
	offset_from_x = SCREEN_WIDTH - (cub->map.sizes.map_lenght \
								* cell_size) - (SCREEN_WIDTH / 80);
	draw_minimap_grid(cub, cell_size, offset_from_x);
	draw_player_n_fov(cub, offset_from_x);
}
