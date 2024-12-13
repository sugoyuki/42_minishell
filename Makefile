NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c executor.c utils.c builtins.c env_manager.c parser.c lexer.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lreadline -lhistory -lncurses -o $(NAME)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
