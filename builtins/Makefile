SRC = cd.c env.c echo.c pwd.c
LINK_LIBFT = -L../libft -lft
CFLAGS = -Wall -Wextra -Werror $(LINK_LIBFT)

all: cd env echo pwd

clean:
	rm -f $(SRC:.c=)

re: clean all