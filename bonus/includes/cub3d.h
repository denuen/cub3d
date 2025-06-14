/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintaur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:24:17 by apintaur          #+#    #+#             */
/*   Updated: 2025/06/14 12:00:18 by apintaur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define WALL 1
# define FLOOR 0
# define BLANK 2
# define DOOR 3
# define CEILING 4

# define LEFT 0
# define RIGHT 1

# define NO_LIGHT 1
# define LIGHT 0

# define SCREEN_WIDTH 1920
# define SCREEN_HEIGHT 1080

# define CELL_SIZE 64
# define FOV 60.0f
# define VIEW_DISTANCE 15.0f
# define RENDER_SCALE 2
# define LOD_THRESHOLD 1.0f
# define TARGET_FPS 60.0f

# define ROT_SPD 0.1f
# define MOVE_SPEED 0.2f

# define MAX_DIST 1e30

# define MOUSE_SPEED 4
# define MOUSE_SENSITIVITY 0.01f
# define LIGHT_INTERVAL 1000

// Minimap definitions
# define MINIMAP_SCALE_FACTOR 100

// Minimap colors
# define MINIMAP_WALL_COLOR 0x333333
# define MINIMAP_FLOOR_COLOR 0xFFFFFFFF
# define MINIMAP_PLAYER_COLOR 0xFF0000
# define FOV_FILL_COLOR 0xFFFF00

// FOV drawing parameters
# define FOV_ANGLE_STEP 0.06f
# define FOV_RADIANS 1.151917f

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

////Parsing structures
typedef struct s_sizes
{
	int		map_height;
	int		map_lenght;
}		t_sizes;

typedef struct s_data
{
	char	*wall;
	char	*door;
	char	*gun1;
	char	*gun2;
	char	*ceiling1;
	char	*ceiling2;
	char	*ceiling3;
	char	*hand;
	char	*floor1;
	char	*floor2;
	char	*floor3;
	char	*floor4;
	char	*floor5;
	char	*floor6;
	char	*floor7;
	char	*floor8;
}		t_data;

typedef struct s_cell_tex
{
	int	floor_tex_idx;
	int	ceiling_tex_idx;
}	t_cell_tex;

typedef struct s_map
{
	t_data		data;
	t_sizes		sizes;
	int			*matrix;
	char		**map;
	int			width;
	int			height;
	int			*doors_state;
}		t_map;

//Rendering structures
typedef struct s_2dpoint
{
	double	x;
	double	y;
}	t_double;
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
	t_image	wall;
	t_image	door;
	t_image	gun_nofire;
	t_image	gun_fire;
	t_image	ceiling_light;
	t_image	ceiling_nolight;
	t_image	hand;
	t_image	floor_light;
	t_image	floor_nolight;
}	t_textures;

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	space;
	int	f;
}	t_keys;

typedef struct s_cub
{
	void		*p;
	t_picture	pic;
	t_map		map;
	t_raycaster	raycaster;
	t_textures	textures;
	t_keys		keys;
	int			frame_count;
	double		last_time;
	double		fps;
	double		fps_accum;
	int			mouse_x;
	int			mouse_times;
	int			gun_animation_frame;
}	t_cub;

//Mlx functions
void			mymlx_pixel_put(t_image *img, int x, int y, int color);
int				mymlx_render(t_cub *cub);
void			mymlx_init(t_cub *cub, char *argv[]);
int				mymlx_exit(t_cub *cub);
int				mymlx_destroy(t_cub *cub);
void			raycaster_init(t_cub *cub);
void			draw_minimap(t_cub *cub);
void			draw_line(t_image *img, int x, t_2ipoint range, \
											unsigned int color);
void			cast_ray(t_ray *ray, t_player *p, t_map *map, int x);
void			key_handler(t_cub *cub);
int				key_press(int keycode, t_cub *cub);
int				key_release(int keycode, t_cub *cub);

void			run_dda_algorithm(t_ray *ray, t_map *map);
void			draw_texture_line(t_cub *cub, t_ray *ray, \
										t_image *w_texture, int x);
void			render_column(t_cub *cub, int x, unsigned int ceiling_color, \
								unsigned int floor_color);

t_image			*get_floor_type(t_cub *cub, int cell_x, int cell_y);
t_image			*get_ceiling_type(t_cub *cub, int cell_x, int cell_y);
void			draw_floor(t_cub *cub, t_ray *ray, t_image *texture, int x);
int				mouse_move(int x, int y, void *param);
void			update_dir(t_cub *cub, int type);

// Funzioni per FPS counter
double			get_time(void);
void			init_fps_counter(t_cub *cub);
void			update_fps_counter(t_cub *cub);

//Matrix-realted functions
void			matrix_creation(t_map *map, int fd, int gnl_calls);

// Rendering utils functions
unsigned int	filter(unsigned int color, float factor);
void			calculate_object_position(t_cub *cub, t_ray *ray, \
								t_2ipoint tmp, t_2fpoint *pos);
t_image			*select_gun_texture(t_cub *cub, int type);
void			draw_gun_pixel(t_cub *cub, t_image *gun, t_2ipoint screen, \
														t_2ipoint init_pos);
void			draw_ceiling(t_cub *cub, t_ray *ray, t_image *texture, int x);

//Parsing functions
int				map_parsing(char *file, t_map *map);
int				get_graphics(char *file, t_map *map);
int				get_map(char *file, int gnl_calls, t_map *map);
int				is_player(char c);
int				save_image(int *i, char **dest, char *line);
int				save_color(int *i, int *dest, char *line);
int				surround_check(char *line, char *prev, char *next, int i);

//Utils
void			check_collisions(t_cub *cub, t_2fpoint new_pos, \
											t_2fpoint old_pos);
void			free_function(char *line);
int				color_helper(char *line, int j);
int				graphics_helper(char *line, int *gnl_calls, int *result, \
														t_map *map);
void			matrix_helper(char *line, int *matrix, int *j, t_map *map);
int				check_helper(char *line, char *prev, char *next, int *player);
void			sizes_helper(char *line, int fd, t_map *map);
char			*gnl_helper(char *line, int gnl_calls, int fd);
void			render_textures(t_cub *cub, int x, t_ray *ray);
void			init_doors_state(t_map *map);
void			toggle_door(t_cub *cub);
int				is_door_open(t_map *map, int x, int y);
int				get_door_index(t_map *map, int x, int y);
void			draw_gun(t_cub *cub, int type);
#endif
