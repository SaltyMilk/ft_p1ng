MSRC = ft_ping.c
CC = gcc
CFLAGS = -Wall -Wextra -Werror
MOBJ = $(MSRC:.c=.o)


NAME = ft_ping

all: libft.a $(NAME)

$(NAME): $(MOBJ)
	gcc $(CFLAGS) libft.a $? -o $(NAME) 

%.o: %.c
	$(CC) -c $(CFLAGS) $?

libft.a:
	make -C libft
	cp ./libft/libft.a libft.a

clean: 
	rm -f $(MOBJ)
	make -C libft clean
fclean:
	rm -f $(MOBJ) $(NAME) libft_malloc.so testx
#libft.a
	make -C libft fclean


re : fclean all

.PHONY : clean fclean re  all