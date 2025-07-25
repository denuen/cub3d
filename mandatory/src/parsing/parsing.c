/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:25:26 by ahabdelr          #+#    #+#             */
/*   Updated: 2025/07/25 15:16:28 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	safe_free(void **p);

// 1 = success, 0 = failure
int	map_parsing(char *file, t_map *map)
{
	int	gnl_calls;
	int	fd;

	gnl_calls = get_graphics(file, map);
	if (!gnl_calls)
		return (0);
	get_next_line(-1);
	fd = open(file, O_RDONLY);
	if (!get_map(file, gnl_calls, map))
	{
		printf("Get map fallita\n");
		close(fd);
		safe_free ((void **) &map->matrix);
		safe_free ((void **) &map->data.nt);
		safe_free ((void **) &map->data.st);
		safe_free ((void **) &map->data.et);
		safe_free ((void **) &map->data.wt);
		return (0);
	}
	close(fd);
	get_next_line(-1);
	fd = open(file, O_RDONLY);
	matrix_creation(map, fd, gnl_calls);
	return (1);
}
