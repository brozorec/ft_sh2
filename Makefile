#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bbarakov <bbarakov@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/02/04 15:43:18 by bbarakov          #+#    #+#              #
#    Updated: 2015/02/04 18:10:41 by bbarakov         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CC=			gcc

CFLAGS=		-Wall -Werror -Wextra

NAME=		ft_minishell2

SRC=		source/main.c \
			source/init.c \
			source/lists.c \
			source/get_next_line.c \
			source/paths.c \
			source/environ.c \
			source/parser.c \
			source/parser_funcs.c \
			source/redirections.c \
			source/execution.c \
			source/execution_funcs.c \
			source/signals.c \
			source/errors.c \
			source/builtins/builtins.c \
			source/builtins/cd_builtin.c \
			source/builtins/cd_builtin_funcs.c \
			source/builtins/cd_builtin_paths.c \
			source/builtins/env_exit_builtins.c \
			source/builtins/pwd_builtin.c \
			source/builtins/setenv_builtin.c \
			source/builtins/unsetenv_builtin.c \

OBJ=		$(SRC:.c=.o)

INCLUDES=	./includes

LIBFT=		./libft -lft

all: lib $(NAME)

$(NAME): $(OBJ)
	@$(CC) -o $@ $^ -I $(INCLUDES) -L $(LIBFT) $(CFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $^ -I $(INCLUDES) $(CFLAGS)

lib:
	@make -C ./libft

.PHONY: clean fclean

clean:
	@make clean -C ./libft
	@rm -rf $(OBJ)

fclean: clean
	@make fclean -C ./libft
	@rm -rf $(NAME)

re: fclean all

