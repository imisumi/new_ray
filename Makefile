# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: imisumi-wsl <imisumi-wsl@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/28 00:51:40 by ichiro            #+#    #+#              #
#    Updated: 2023/09/09 15:29:55 by imisumi-wsl      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minirt

# HEADER = lib/MLX42/include/MLX42/MLX42.h

CFLAGS = 

INCLUDES =	includes/main.h			\
			includes/array.h		\
			includes/darray.h		\
			lib/MLX42/include/MLX42/MLX42.h \
			lib/libft/includes/libft.h \
			lib/lib3d/includes/lib3d.h

CFLAGS += $(addprefix -I,$(sort $(dir $(INCLUDES))))
CFLAGS +=

LEAKS = -fsanitize=address

cc = gcc

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
		src/array.c						\
		src/darray.c						


all: $(LIBFT) $(MLX) $(NAME)
	@echo "$(GREEN)[Completed minirt]$(NC)"

$(LIBFT):
	@$(MAKE) -C lib/libft

$(LIB3D):
	@$(MAKE) -C lib/lib3d

$(MLX):
	@echo "$(BLUE)[Compiling MLX]$(NC)"
	@cd ./lib/MLX42 && cmake -DBUILD_TESTS=ON -B build && cmake --build build --parallel
	@echo "$(GREEN)[Completed MLX]$(NC)"

$(NAME): $(MLX) $(LIBFT) $(LIB3D) $(OBJS)
	$(cc) $(CFLAGS) $(SRCS) $(MLX) $(LIBFT) $(LIB3D) $(LFLAGS) -o $(NAME)

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
