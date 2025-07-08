NAME = minishell
CC = cc
FLAGS = -Werror -Wextra -Wall

LIBFT_PATH = libs/libft
LIBFT = $(LIBFT_PATH)/libft.a

SOURCES = parser/quote_handler.c\
		parser/tokenizer.c\
		utils/utils.c\
		main.c\