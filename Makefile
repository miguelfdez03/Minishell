NAME = minishell
CC = cc
FLAGS = -Werror -Wextra -Wall

LIBFT_PATH = libs/libft
LIBFT = $(LIBFT_PATH)/libft.a

SOURCES = src/parser/checker.c\
		src/parser/tokenizer.c\
		src/utils/utils.c\
		src/main.c\
		