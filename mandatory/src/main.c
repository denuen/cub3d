/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 08:38:20 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/12 19:14:40 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	main(int argc, char **argv)
{
	t_cub	cub;

	if (argc != 2)
	{
		ft_printf("Error: Usage: %s <map_file.cub>\n", argv[0]);
		return (1);
	}
	mymlx_init(&cub, argv);
	mlx_hook(cub.pic.win.p, 2, 1L << 0, key_press, &cub);
	mlx_hook(cub.pic.win.p, 3, 1L << 1, key_release, &cub);
	mlx_hook(cub.pic.win.p, 17, 0, mymlx_exit, &cub);
	mlx_loop_hook(cub.p, key_handler, &cub);
	mymlx_render(&cub);
	mlx_loop(cub.p);
	return (0);
}
