NAME = minishell

#dir
LIBFT_DIR = libft
SRC_DIR = src
OBJ_DIR := obj

#Add new files here
SRC = $(shell find ./src -iname "*.c")
OBJ = $(SRC:./$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

OS ?= $(shell uname)
FLAGS = -Werror -Wextra -Wall
#to use readline
RLFLAG = -lreadline 
HEADERS := -Iinclude -I$(LIBFT_DIR)

ifeq ($(OS),Darwin)
	RL_DIR = /opt/homebrew/Cellar/readline/8.3.1
	RLFLAG += -L$(RL_DIR)/lib
	HEADERS += -I$(RL_DIR)/include
endif

LIBFT = $(LIBFT_DIR)/libft.a

# ========== Testing SRC ============
# lexing module
LEX_SRC = src/lexing/lexing.c \
			src/lexing/main.c \
			src/lexing/lexing_error.c \
			src/lexing/lexing_free.c \
			src/lexing/init_env.c

all: buildlib $(NAME)

# %.o: %.c
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	cc $(FLAGS) $(HEADERS) -c $< -o $@ 

$(NAME): $(OBJ)
	cc $(HEADERS) $(FLAGS) $(OBJ) $(LIBFT) $(RLFLAG) -o $@

buildlib:
	$(MAKE) -C $(LIBFT_DIR)

clean:
	@$(MAKE) -s -C $(LIBFT_DIR) clean
	rm -f $(OBJ)

fclean: clean
	@$(MAKE) -s -C $(LIBFT_DIR) fclean
	@rmdir $(OBJ_DIR)
	rm -f $(NAME)

re: fclean all

# ================= Test rules ===================
#  Example: make env -> compile only env/*.c + test_env.c into ./env executable
lex: buildlib
	@echo "Compiling lex testing module..."
	cc $(FLAGS) $(HEADERS) $(LEX_SRC) $(LIBFT) $(RLFLAG) -o lex
	@echo "✅ Built test executable: ./lex"

.PHONY: all buildlib clean fclean re