NAME		= philo
CC 			= gcc 
CFLAGS 		= -Wall -Wextra -Werror -lpthread -fsanitize=thread -g3
SRC_PATH 	= sources/
OBJ_PATH 	= objects/

SRC			= main.c \
			  parsing.c \
			  setup.c \
			  start_dinner.c \
			  critical_section.c \
			  utils.c 

SRCS		= $(addprefix $(SRC_PATH), $(SRC))
OBJ			= $(SRC:.c=.o)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ))

INC			= -I ./include/


all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean