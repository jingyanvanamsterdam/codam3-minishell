NAME = minishell

#dir
LIBFT_DIR = libft
SRC_DIR = src
OBJ_DIR := obj

#Add new files here
SRC = $(shell find ./src -iname "*.c")
OBJ = $(SRC:./$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

OS ?= $(shell uname)
FLAGS = -Werror -Wextra -Wall -fsanitize=address
#to use readline
RLFLAG = -lreadline 
HEADERS := -Iinclude -I$(LIBFT_DIR)

ifeq ($(OS),Darwin)
# 	RL_DIR = /opt/homebrew/Cellar/readline/8.3.1
	RL_DIR = $(shell brew --prefix readline)
	RLFLAG += -L$(RL_DIR)/lib
	HEADERS += -I$(RL_DIR)/include
endif

LIBFT = $(LIBFT_DIR)/libft.a

# ========== Testing SRC ============
# env module
ENV_SRC = $(shell find ./src/env -iname "*.c")
# utils module
UTILS_SRC = $(shell find ./src/utils -iname "*.c")
# lexing module
LEX_SRC = $(shell find ./src/lexing -iname "*.c")
PARSE_SRC = $(shell find ./src/parse -iname "*.c")
# execution module
EXECUTION_SRC = $(shell find ./src/execution -iname "*.c")
# builtins module
BUILTINS_SRC = $(shell find ./src/builtins -iname "*.c")

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
env: buildlib
	@echo "Compiling env testing module..."
	cc $(FLAGS) $(HEADERS) $(ENV_SRC) $(UTILS_SRC) $(LIBFT) -o env
	@echo "✅ Built test executable: ./env"
lex: buildlib
	@echo "Compiling lex testing module..."
	cc $(FLAGS) $(HEADERS) $(LEX_SRC) $(LIBFT) $(RLFLAG) -o lex
	@echo "✅ Built test executable: ./lex"
parse: buildlib
	@echo "Compiling parse testing module..."
	cc $(FLAGS) $(HEADERS) $(LEX_SRC) $(PARSE_SRC) $(ENV_SRC) $(UTILS_SRC) $(LIBFT) $(RLFLAG) -o parse
	@echo "✅ Built test executable: ./parse"
execution: buildlib
	@echo "Compiling execution testing module..."
	cc $(FLAGS) $(HEADERS) $(LEX_SRC) $(PARSE_SRC) $(BUILTINS_SRC) $(EXECUTION_SRC) $(ENV_SRC) $(UTILS_SRC) $(LIBFT) $(RLFLAG) -o execution
	@echo "✅ Built test executable: ./execution"

.PHONY: all buildlib clean fclean re env lex parse execution
