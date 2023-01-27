CC			=  g++
CFLAGS		=  --std=c++98
LDFLAGS		=  -lc
WFLAGS		=  -Wall -Wextra -Werror
DEBUG		=  -g3
OPTFLAGS	=  -o3

NAME		=  libcontainers.a

INC_DIR		=  inc
SRC_DIR		=  src
OBJ_DIR		=  obj

INC_HEADERS = inc.h
HEADERS		=  $(INC_DIR)/$(INC_HEADERS)

CFLAGS		+= -I$(DIR_INC)
OBJ			=  $(DIR_OBJ)/$(SRC:.c=.o)

all: $(DIR_OBJ) $(NAME)

clean: rm -rf $(DIR_OBJ)

fclean: clean
        rm -rf $(NAME)

re: fclean all

$(DIR_OBJ):
	mkdir -p $@

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c $(INC_HEADERS) Makefile
	$(CC) $(CFLAGS) $(DEBUG) $(HEADERS) $< -c -o $@

$(NAME): $(OBJ) $(HEADERS) Makefile
	ar -rc $@ $(OBJ)
	ranlib $@

bench: $(NAME)
	$(CC) -L. -lcontainers benchmarks/main.cpp

.PHONY: all clean fclean re
