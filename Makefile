# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gmayweat <gmayweat@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/25 22:40:29 by gmayweat          #+#    #+#              #
#    Updated: 2021/07/27 08:19:40 by gmayweat         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

SRCS =					main.c\
						heredoc.c\
						pipex.c\
						cmd_alloc.c

HEAD =					pipex.h

HEADPATH = $(addprefix includes/, $(HEAD))

OBJS = $(SRCS:.c=.o)

OBJDIR = objs
			

OBJSPATH = $(addprefix $(OBJDIR)/, $(OBJS))

FLAGS = -g -Wall -Wextra -Werror -Ilibft -Iincludes #-fsanitize=address

vpath %.o objs
vpath %.c srcs
vpath %.h includes

.Phony: all $(NAME) clean fclean libft.a re $(OBJDIR)

all: libft.a $(NAME)

libft.a:
	$(MAKE) -C libft

$(NAME): $(OBJDIR) $(OBJS)
	clang $(FLAGS) -o $(NAME) $(OBJSPATH) libft/libft.a

%.o: %.c $(HEADPATH)
	clang $(FLAGS) -o $(addprefix objs/, $@) -c $<

bonus:
	$(MAKE) SRCS='$(SRCS:.c=_bonus.c)' HEAD='$(HEAD:.h=_bonus.h)'

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	$(MAKE) -C libft clean
	rm -rf $(OBJDIR)

fclean:
	$(MAKE) -C libft fclean
	rm -rf $(OBJDIR) $(NAME)

re: fclean all
