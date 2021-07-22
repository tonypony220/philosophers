#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>


int	ft_atoi(char *str)
{
	int		sign;
	long	num;

	num = 0;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	sign = 1 - 2 * (*str == '-' && 1);
	*str == '-' && str++;
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + *str++ - '0';
		if (num < 0)
			return ((sign > 0) * -1);
	}
	return ((int)num * sign);
}

struct s_vars 
{
    pthread_mutex_t **mutexes;
    pthread_mutex_t *mtx;
	int				die;
	int				eat;
	int				sleep;
	int				meal_num;
	int				num;
};

int print_status(struct s_vars *data, char *status)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	printf("%ld %d %s\n", current_time.tv_usec / 10000, data->num, status);
}

void save_data_to_thread(struct s_vars *data, struct s_vars *data_2)
{
    data->mutexes	= data_2->mutexes;
	data->die		= data_2->die;
	data->eat		= data_2->eat;
	data->sleep		= data_2->sleep;
	data->meal_num	= data_2->meal_num;
	data->num		= data_2->num;
	data->mtx		= data_2->mtx;
}

/* i: philo number */
void *worker_thread(void *v_data)
{
	struct s_vars data;	
		
	save_data_to_thread(&data, v_data);
	print_status(&data, "started\n");
    pthread_mutex_unlock(data.mtx);

////	while (1)
////	{
////        pthread_mutex_lock(mutexes);
////        pthread_mutex_unlock(mutexes);
////    }
    return (0);  /* never reached */
}

int lunch(struct s_vars *var, int n)
{
    pthread_t	thr;
	int			i;

	i = -1;
	var->mutexes = malloc(sizeof(pthread_mutex_t) * (n + 1));
	memset(var->mutexes, 0, sizeof(pthread_mutex_t) * (n + 1));
	while (++i < n)	
		var->mutexes[i] = pthread_mutex_init(thr, NULL);
	i = -1;
	var->mtx = pthread_mutex_init(thr, NULL);
	while (++i < n)	
	{
		var->num = i;
        pthread_mutex_lock(var->mtx);
		pthread_create(&thr, NULL, worker_thread, var);
	}
}

/* args:
 * 		1 number_of_philosophers
 * 		2 time_to_die 
 * 		3 time_to_eat
 * 		4 time_to_sleep 
 * 		5 number_of_times_each_philosopher_must_eat
 */
int _main(int ac, char **av)
{
	struct s_vars var;

	if (!(ac == 5 || ac == 4))
	{
		printf("error arg num\n");
		return (1);
	}
	var.die = ft_atoi(av[2]);
	var.eat = ft_atoi(av[3]);
	var.sleep = ft_atoi(av[4]);
	var.meal_num = 0;
	ac == 6 && (var.meal_num = ft_atoi(av[5]));
	lunch(&var, ft_atoi(av[1]));	
	printf("end\n");
	return (1);
}

int main(int ac, char **av)
{
	return(_main(ac, av));
}
