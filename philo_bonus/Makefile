SRCS	=	exit.c  loop_thread.c  utils.c sem_philo.c tracking.c

OBJS	=	${SRCS:.c=.o}

CFLAGS	=   -O0 -Wall -Wextra -Werror #-g -fsanitize=address


HEAD	=	philo.h

CC		=	clang

RM		=	-rm	-f

NAME	=	philo_bonus	

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
