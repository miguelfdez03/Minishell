NAME = libft.a

SOURCES = ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_strlen.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_strlcpy.c \
		ft_strlcat.c \
		ft_tolower.c \
		ft_toupper.c \
		ft_strchr.c \
		ft_strrchr.c \
		ft_strncmp.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_strnstr.c \
		ft_atoi.c \
		ft_calloc.c \
		ft_strdup.c \
		ft_substr.c \
		ft_strjoin.c \
		ft_strtrim.c \
		ft_split.c \
		ft_itoa.c \
		ft_strmapi.c \
		ft_striteri.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_lstsize_bonus.c\
		ft_lstadd_back_bonus.c\
		ft_lstadd_front_bonus.c\
		ft_lstclear_bonus.c\
		ft_lstdelone_bonus.c\
		ft_lstiter_bonus.c\
		ft_lstlast_bonus.c\
		ft_lstmap_bonus.c\
		ft_lstnew_bonus.c\
		ft_printf.c\
		ft_printf_utils.c\
		get_next_line.c\
		get_next_line_utils.c\
		ft_error.c\

OBJECTS = $(SOURCES:.c=.o)

GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RED = \033[0;31m
RESET = \033[0m

OK = ✅
BUILD = 🔨
CLEAN = 🧹

CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME) 
$(NAME): $(OBJECTS)
	@$(AR) -r $@ $?
	@echo "$(BLUE)$(OK) Library compiled: $(NAME)$(RESET)"

%.o: %.c
	@$(CC) -c $(CFLAGS) $<
	@echo "$(GREEN)$(BUILD) Compiling $<$(RESET)"

clean:
	@rm -f $(OBJECTS)
	@echo "$(YELLOW)$(CLEAN) Object files removed$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(CLEAN) Binary removed: $(NAME)$(RESET)"

re: fclean all
