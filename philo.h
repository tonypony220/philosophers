#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# define HUMAN_NUM 1
# define PROTECT_PRINT 1
# define HARD_EXIT 0
# define COLOR 1
# define DONE 1
# define JOINED 2
# define DEAD 3
# define QUIT -1
# define RED 	"\033[1;31m"
# define GREEN   "\033[0;32m"
# define YELLOW	"\033[1;33m"
# define BLUE	"\033[1;34m"
# define PURPLE  "\033[1;35m"
# define CYAN    "\033[1;36m"
# define RESET   "\033[0;0m"
# define BOLD    "\033[;1m"
# define REVERSE "\033[;7m"
# define MISALIGNMENT 0.992

struct s_vars
{
	pthread_mutex_t	*mutexes;
	pthread_mutex_t	*mtx;
	int				die;
	int				eat;
	int				sleep;
	int				meal_num;
	int				num;
	pthread_t		*ids;
	struct timeval	global_start;
	struct timeval	*start;
	int				philos;
	char			*status;
	int				fst;
	int				sec;
};

struct s_track
{
	pthread_t		thr;
	char			status;
	int				joined;
	char			exiting;
	struct timeval	end;
	struct timeval	start;
};

void				*worker_thread(void *v_data);
int					ft_atoi(char *str);
int					sleeping(long amount);
long				get_time(struct timeval *time);
long				diff(struct timeval *end, struct timeval *start);
int					print_status(struct s_vars *data, char *status, int i);
void				*worker_thread(void *v_data);
int					tracking(struct s_vars *var, int n);
int					exiting(struct s_vars *var, int n, int code);

#endif
