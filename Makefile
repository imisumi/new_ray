# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imisumi <imisumi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/28 00:51:40 by ichiro            #+#    #+#              #
#    Updated: 2023/10/24 16:29:12 by imisumi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minirt

# HEADER = lib/MLX42/include/MLX42/MLX42.h

CFLAGS = 

INCLUDES =	includes/main.h			\
			includes/darray.h		\
			lib/MLX42/include/MLX42/MLX42.h \
			lib/libft/includes/libft.h \
			lib/lib3d/includes/lib3d.h

CFLAGS += $(addprefix -I,$(sort $(dir $(INCLUDES))))
CFLAGS +=

LEAKS = -fsanitize=address

cc = gcc -msse4.2 -O3 -march=native

# cc += -pg

LFLAGS = -ldl -lglfw -pthread -lm 

MLX = lib/MLX42/build/libmlx42.a
LIBFT = lib/libft/libft.a
LIB3D = lib/lib3d/lib3d.a

OBJS_DIR = objs
SRC_DIR = src

GREEN := \033[1;32m
RED := \033[1;31m
BLUE := \033[1;34m
PINK := \033[1;38;5;206m
NC := \033[0m


SRCS =	src/main.c						\
		src/darray.c					\
		src/camera.c					\
		src/intersection.c				\
		src/scene.c						\
		src/cube.c \
		src/obj.c


all: $(LIBFT) $(LIB3D) $(MLX) $(NAME)
	@echo "$(GREEN)[Completed minirt]$(NC)"

$(LIBFT):
	@$(MAKE) -C lib/libft

$(LIB3D):
	git clone --recursive git@github.com:imisumi/lib3d.git lib/lib3d
	git submodule update --init --recursive lib/MLX42
	@$(MAKE) -C lib/lib3d

$(MLX):
	git submodule update --init --recursive lib/MLX42
	@echo "$(BLUE)[Compiling MLX]$(NC)"
	@cd ./lib/MLX42 && cmake -B build && cmake --build build --parallel
	@echo "$(GREEN)[Completed MLX]$(NC)"

$(NAME): $(LIBFT) $(LIB3D) $(MLX) $(OBJS)
	$(cc) $(CFLAGS) $(SRCS) $(LIBFT) $(LIB3D) $(MLX) $(LFLAGS) -o $(NAME)

run: re
	./$(NAME)

clean:
	@rm -f libmlx.dylib
	@make clean -C lib/libft
	@echo "$(RED)[Deleted $(NAME) objects]$(NC)"

fclean:
	@rm -rf $(NAME)
	@make fclean -C lib/libft
	@echo "$(RED)[Deleted $(NAME) objects]$(NC)"
	@echo "$(RED)[Deleted $(NAME)]$(NC)"

mlxclean:
	@rm -rf ./lib/MLX42/build
	@echo "$(RED)[Deleted MLX]$(NC)"

re: fclean all

.PHONY: all clean fclean re
