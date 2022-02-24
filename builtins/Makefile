SRC = cd.c
LINK_LIBFT = -L../libft -lft
CFLAGS = -Wall -Wextra -Werror

all: cd

cd:
	gcc $(CFLAGS) cd.c $(LINK_LIBFT) -o cd

clean:
	rm -f $(SRC:.c=)
re: clean all