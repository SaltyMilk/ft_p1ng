MSRC = ft_ping.c err.c
CC = gcc
CFLAGS = -Wall -Wextra -Werror
MOBJ = $(MSRC:.c=.o)


NAME = ft_ping

all: $(NAME)

$(NAME): $(MOBJ) libft/libft.a
	gcc $(CFLAGS) $? libft/libft.a -o $(NAME) 

%.o: %.c
	$(CC) -c $(CFLAGS) $?

libft/libft.a:
	make -C libft

clean: 
	rm -f $(MOBJ)
	make -C libft clean
fclean:
	rm -f $(MOBJ) $(NAME) 
	make -C libft fclean


re : fclean all

.PHONY : clean fclean re  all