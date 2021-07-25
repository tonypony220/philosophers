#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
/* O_CREAT, O_EXEC */
# include <fcntl.h>
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <errno.h>          /* errno, ECHILD            */
# define HUMAN_NUM 1
# define PROTECT_PRINT 1
# define SHOW_STARVING 0
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
# define MISALIGNMENT 0.996
# define SEM_FORKS "forks"
# define SEM_DISPLAY "disp"
# define SEM_WAITER "waiter"
# define SEM_EXIT "exiting"
# define ERR_MALLOC 1
# define ERR_SEM 2
# define ERR_FORK 3
# define ERR_ARG 4

struct s_vars
{
	sem_t			*sem_forks;
	sem_t			*sem_disp;	
	sem_t			*sem_waiter;	
	sem_t			*sem_exit;	
	int				die;
	int				eat;
	int				sleep;
	int				meal_num;
	int				num;
	struct timeval	global_start;
	struct timeval	start;
	int				philos;
	char			status;
	pid_t			*pids;
};

struct s_track
{
	char			status;
	char			exiting;
	struct timeval	end;
	struct timeval	start;
};

int					run_loop(struct s_vars *vars);
void				*worker_thread(void *v_data);
int					ft_atoi(char *str);
int					sleeping(long amount);
long				get_time(struct timeval *time);
long				diff(struct timeval *end, struct timeval *start);
int					print_status(struct s_vars *data, char *status, int i);
void				*worker_thread(void *v_data);
void				*tracking(void *v_data);
int					exiting(struct s_vars *var, int code);
int kill_all(pid_t *pids);

#endif
