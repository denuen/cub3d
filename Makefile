MANDATORY_NAME			= cub3d
BONUS_NAME				= cub3d_bonus
CC						= gcc
CFLAGS					= -g -Wall -Wextra -Werror

SRC_DIR_MANDATORY	= mandatory/src/
SRC_DIR_BONUS		= bonus/src/
RND_DIR				= rendering/
PRS_DIR				= parsing/
H_DIR					= includes
LIBFT_DIR				= libft
MLX_DIR					= .minilibx/minilibx-linux

C_FILES_MANDATORY		= main.c
RND_FILES_MANDATORY		= raycaster.c raycaster_utils.c mma.c raycaster_draws.c mymlx.c mymlx_utils.c wall_check.c
PRS_FILES_MANDATORY		= map_save.c gfx_save.c matrix.c parsing.c general_utils.c map_utils.c

C_FILES_BONUS			= main_bonus.c
RND_FILES_BONUS			= mymlx_bonus.c mymlx_utils_bonus.c fps_bonus.c minimap_bonus.c minimap_utils_bonus.c wall_collisions_bonus.c raycaster_bonus.c raycaster_dda_bonus.c raycaster_draws_bonus.c raycaster_draws_utils_bonus.c raycaster_init_bonus.c mouse_move_bonus.c door_system_bonus.c key_handler.c
PRS_FILES_BONUS			= map_save_bonus.c gfx_save_bonus.c matrix_bonus.c parsing_bonus.c general_utils_bonus.c map_utils_bonus.c gfx_utils_bonus.c

SRCS_MANDATORY			= $(C_FILES_MANDATORY) $(addprefix $(RND_DIR), $(RND_FILES_MANDATORY)) $(addprefix $(PRS_DIR), $(PRS_FILES_MANDATORY))
SRCS_BONUS				= $(C_FILES_BONUS) $(addprefix $(RND_DIR), $(RND_FILES_BONUS)) $(addprefix $(PRS_DIR), $(PRS_FILES_BONUS))

OBJS_MANDATORY			= $(addprefix $(SRC_DIR_MANDATORY), $(SRCS_MANDATORY:.c=.o))
OBJS_BONUS				= $(addprefix $(SRC_DIR_BONUS), $(SRCS_BONUS:.c=.o))

MLX_LIB					= $(MLX_DIR)/libmlx.a
LIBFT_LIB				= $(LIBFT_DIR)/libft.a

all: $(MANDATORY_NAME)

$(MANDATORY_NAME): $(OBJS_MANDATORY) $(MLX_LIB) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJS_MANDATORY) -o $(MANDATORY_NAME) -L $(MLX_DIR) -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -L $(LIBFT_DIR) -lft

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(OBJS_BONUS) $(MLX_LIB) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(BONUS_NAME) -L $(MLX_DIR) -lmlx_Linux -L/usr/lib -lXext -lX11 -lm -L $(LIBFT_DIR) -lft

$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR)

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

$(SRC_DIR_MANDATORY)%.o: $(SRC_DIR_MANDATORY)%.c $(H_DIR)/cub3d.h
	$(CC) $(CFLAGS) -I $(H_DIR) -I $(LIBFT_DIR)/includes -I $(MLX_DIR) -c $< -o $@

$(SRC_DIR_BONUS)%.o: $(SRC_DIR_BONUS)%.c $(H_DIR)/cub3d.h
	$(CC) $(CFLAGS) -I $(H_DIR) -I $(LIBFT_DIR)/includes -I $(MLX_DIR) -c $< -o $@

clean:
	rm -f $(OBJS_MANDATORY)
	$(MAKE) clean -C $(LIBFT_DIR)
	$(MAKE) clean -C $(MLX_DIR)

clean_bonus:
	rm -f $(OBJS_BONUS)
	$(MAKE) clean -C $(LIBFT_DIR)
	$(MAKE) clean -C $(MLX_DIR)

fclean: clean
	rm -f $(MANDATORY_NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)

fclean_bonus: clean_bonus
	rm -f $(BONUS_NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

re_bonus: fclean_bonus bonus

.PHONY: all bonus clean clean_bonus fclean fclean_bonus re re_bonus
