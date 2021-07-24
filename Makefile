SRCS	=	exit.c philo.c loop_thread.c tracking.c utils.c

OBJS	=	${SRCS:.c=.o}

CFLAGS	=   -O0 -Wall -Wextra -Werror

HEAD	=	philo.h

CC		=	clang

RM		=	-rm	-f

NAME	=	philo

%.o: %.c $(HEAD)
	@$(CC) ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
		$(CC) ${CFLAGS} ${OBJS} -o ${NAME}
clean:
	rm -f $(OBJS)

fclean:	clean
	rm -f $(NAME)

re:		fclean all

.PHONY:	all bonus clean fclean re
