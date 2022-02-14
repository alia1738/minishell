NAME = libft.a
CFILES = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_isspace.c ft_strlen.c \
		 ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c \
		 ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c \
		 ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_strjoin.c ft_strtrim.c \
		 ft_substr.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
		 ft_putendl_fd.c ft_putnbr_fd.c ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
		 ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c
OBJECTS = $(CFILES:.c=.o)
DEP = $(patsubst %.o,%.d,$(OBJECTS))
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

-include $(DEP)
$(NAME): $(OBJECTS)
	ar rc $(NAME) $(OBJECTS)
%.o: %.c
	gcc -c -MD $(CFLAGS) $<
clean:
	rm -rf $(OBJECTS) $(DEP)
fclean:
	rm -rf $(OBJECTS) $(DEP) $(NAME)
bonus: all
re: fclean all
.PHONY: all bonus clean fclean re