/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_move_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:34:38 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/14 12:07:05 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#define LEFT 0
#define RIGHT 1

int	mouse_move(int x, int y, void *param)
{
	t_cub	*data;

	data = (t_cub *)param;
	(void)y;
	if (!data)
		return (0);
	if (data->mouse_x == 0 && x != 0)
	{
		data->mouse_x = x;
		return (0);
	}
	data->mouse_times++;
	if (x > data->mouse_x && data->mouse_times > 0 \
		&& !(data->mouse_times % MOUSE_SPEED))
	{
		update_dir(data, RIGHT);
	}
	else if (x < data->mouse_x && data->mouse_times > 0 \
		&& !(data->mouse_times % MOUSE_SPEED))
	{
		update_dir(data, LEFT);
	}
	data->mouse_x = x;
	return (0);
}
