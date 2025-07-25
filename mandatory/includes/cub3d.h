/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:29:10 by apintaur          #+#    #+#             */
/*   Updated: 2025/07/25 11:32:56 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WALL 1
# define FLOOR 0
# define BLANK 2
# define LEFT 0
# define RIGHT 1
# define SCREEN_WIDTH 1920
# define SCREEN_HEIGHT 1080

# define CELL_SIZE 64
# define FOV 60.0f
# define VIEW_DISTANCE 15.0f
# define RENDER_SCALE 2
# define LOD_THRESHOLD 1.0f

# define ROT_SPD 0.1f
# define MOVE_SPEED 0.2f

# include "../../libft/includes/libft.h"
# include "../../.minilibx/minilibx-linux/mlx.h"
# include "../../.minilibx/minilibx-linux/mlx_int.h"

# include <math.h>
# include <X11/X.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>

//Mlx structures
typedef struct s_size
{
	int	width;
	int	height;
}	t_size;

typedef struct s_image
{
	void			*p;
	void			*addr;
	int				bits_pp;
	int				lenght;
	int				endian;
	unsigned int	color;
	t_size			size;
}	t_image;

typedef struct s_window
{
	void			*p;
	unsigned int	color;
	t_size			size;
}	t_window;

typedef struct s_picture
{
	t_image		img;
	t_window	win;
}	t_picture;

//Parsing structures
typedef struct s_sizes
{
	int		map_height;
	int		map_lenght;
}		t_sizes;

typedef struct s_data
{
	int		floor;
	int		ceiling;
	char		*nt;
	char		*st;
	char		*et;
	char		*wt;
}		t_data;

typedef struct s_map
{
	t_data		data;
	t_sizes		sizes;
	int			*matrix;
}		t_map;

//Rendering structures
typedef struct s_2fpoint
{
	float	x;
	float	y;
}	t_2fpoint;

typedef struct s_2ipoint
{
	int		x;
	int		y;
}	t_2ipoint;

typedef struct s_player
{
	t_2fpoint	pos;
	t_2fpoint	dir;
	t_2fpoint	plane;
}	t_player;

typedef struct s_ray
{
	t_2fpoint	dir;
	t_2fpoint	side_dist;
	t_2fpoint	delta_dist;
	t_2ipoint	cell_pos;
	t_2ipoint	step;
	float		perp_wall_dist;
	int			side;
	int			line_height;
	int			draw_start;
	int			draw_end;
}	t_ray;

typedef struct s_raycaster
{
	t_player	player;
	t_ray		*rays;
}	t_raycaster;

typedef struct s_wall
{
	t_image	north;
	t_image	south;
	t_image	west;
	t_image	east;
}	t_wall;

typedef struct s_textures
{
	t_wall	wall;
}	t_textures;

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
}	t_keys;

typedef struct s_cub
{
	void		*p;
	t_picture	pic;
	t_map		map;
	t_raycaster	raycaster;
	t_textures	textures;
	t_keys		keys;
}	t_cub;

//Mlx functions
void	mymlx_pixel_put(t_image *img, int x, int y, int color);
void	mymlx_init(t_cub *cub, char *argv[]);
int		mymlx_destroy(t_cub *cub);
int		mymlx_render(t_cub *cub);
int		mymlx_render(t_cub *cub);
int		mymlx_exit(t_cub *cub);

//Mlx key handlers functions
int		key_release(int keycode, t_cub *cub);
int		key_press(int keycode, t_cub *cub);
int		key_handler(t_cub *cub);

//Parsing functions
void	matrix_creation(t_map *map, int fd, int gnl_calls);
int		surround_check(char *line, char *prev, char *next, int i);
int		get_map(char *file, int gnl_calls, t_map *map);
int		save_image(int *i, char **dest, char *line);
int		save_color(int *i, int *dest, char *line);
int		get_graphics(char *file, t_map *map);
int		map_parsing(char *file, t_map *map);
int		is_player(char c);

//Parsing utils functions
void	matrix_helper(char *line, int *matrix, int *j, t_map *map);
void	sizes_helper(char *line, int fd, t_map *map);
void	safe_free(void **p);
int		graphics_helper(char *line, int *gnl_calls, int *result, t_map *map);
int		check_helper(char *line, char *prev, char *next, int *player);
int		color_helper(char *line, int j);
char	*gnl_helper(char *line, int gnl_calls, int fd);

//Rendering functions
void	cast_ray(t_ray *ray, t_player *p, t_map *map, int x);
void	raycaster_init(t_cub *cub);
void	render_column(t_cub *cub, int x,
			unsigned int ceiling_color, unsigned int floor_color);
#endif
