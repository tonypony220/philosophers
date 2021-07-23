#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#define PROTECT_PRINT 0

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
	struct timeval	*start;
    pthread_t		*ids; // not needed
	struct timeval	global_start;
	int				philos;
};


long	get_time(struct timeval *time)
{
	return (time->tv_sec * 1000000 + time->tv_usec);
}

int print_status(struct s_vars *data, char *status)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);

	PROTECT_PRINT && pthread_mutex_lock(data->mtx);
	printf("%ldms #%d %s\n", (get_time(&current_time) - get_time(&data->global_start)) / 1000, data->num , status);
    PROTECT_PRINT && pthread_mutex_unlock(data->mtx);
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
	data->start		= data_2->start;
	data->global_start	= data_2->global_start;
	data->philos	= data_2->philos;
}


/* i: philo number */
void *worker_thread(void *v_data)
{
	struct s_vars	data;	
	int				fork;
	int				fst;
	int 			sec;
	struct timeval	end;

	save_data_to_thread(&data, v_data);
	gettimeofday(&data.start[data.num], NULL);
	pthread_mutex_unlock(data.mtx);
	// 0  0  1 
	// 1  1  2
	// 2  2  3
	// 3  3  4
	// 4  4  0
	fst = (data.num + (data.num % 2)) % data.philos; // && usleep(200);
	sec = (data.num + !(data.num % 2)) % data.philos; // && usleep(200);
	
	///sec = !(fst) && 1;
	//data.num == 1 && (sec = 1) && (fst = 0);
	//fst = 0;
	//sec = 1;
	printf("phil %d first %d, second %d\n", data.num, fst, sec);
	
	////////print_status(&data, "started\n");
	/////pthread_mutex_unlock(((struct s_vars *)v_data)->mtx);
	while (1)
	{
        pthread_mutex_lock(&data.mutexes[fst]);
		printf("taken #%d fork=%d  ", data.num , fst);
		print_status(&data, "has taken a fork");
        pthread_mutex_lock(&data.mutexes[sec]);
		printf("taken #%d fork=%d  ", data.num , sec);
		print_status(&data, "has taken a fork");
		print_status(&data, "eating");
		usleep(data.eat * 1000);
    	pthread_mutex_unlock(&data.mutexes[sec]);
		printf("released #%d fork=%d \n", data.num,  sec);
      	pthread_mutex_unlock(&data.mutexes[fst]);
		printf("released #%d fork=%d \n", data.num,  fst);
	//	gettimeofday(&end, NULL);
	//	pthread_mutex_lock(data.mtx);
		gettimeofday(&data.start[data.num], NULL);
	//	pthread_mutex_unlock(data.mtx);
		print_status(&data, "is sleeping");
		usleep(data.sleep * 1000);
		print_status(&data, "is thinking");
	//	while (1)
	//	{
	//		gettimeofday(&end, NULL);
	//		printf("%d %ld << \n", data.num + 1, (get_time(&end) - get_time(&data.start[data.num])) / 1000);
	//		if ((get_time(&end) - get_time(&data.start[data.num])) > data.die * 400)
	//		{
	//			break ;
	//		}
	//		usleep(data.die / 8 * 1000);
	//	}
		//start = var->start[i];
			
//		printf("#%d diff=%ld die=%d\n",
//				data.num,
//				((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000,
//				data.die);
//		if (((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) / 1000 > data.die)
//		{
//			pthread_mutex_lock(data.mtx);
//			data.ids[data.num]++;
//			pthread_mutex_unlock(data.mtx);
//			break;
//		}
	//	if (end.tv_usec - start.tv_usec > data.eat)
	//		break;
		//usleep(data. * 1000);
    }
	print_status(&data, "died");
    return (0);  /* never reached */
}



int lunch(struct s_vars *var, int n)
{
    pthread_t	thr;
	int			i;

	i = -1;
	//printf("size of pthread%ld %ld<<<<<:\n", sizeof(a), sizeof(pthread_mutex_t));

	var->mutexes = malloc(sizeof(pthread_mutex_t) * (n + 0));
	memset(var->mutexes, 0, sizeof(pthread_mutex_t) * (n + 0));

	var->mtx = malloc(sizeof(pthread_mutex_t));
	memset(var->mtx, 0, sizeof(pthread_mutex_t));

	var->ids = malloc(sizeof(pthread_t) * n);
	memset(var->ids, 0, sizeof(pthread_t));

	var->start = malloc(sizeof(struct timval*) * n);
	memset(var->ids, 0, sizeof(struct timval*) * n);

	pthread_mutex_init(var->mtx, NULL);

//	while (++i < n)	
//	i = -1;
	gettimeofday(&var->global_start, NULL);
	while (++i < n)	
	{
		pthread_mutex_init(&var->mutexes[i], NULL);
		pthread_mutex_lock(var->mtx);
		var->num = i;
	//	gettimeofday(&var->start[i], NULL);
		pthread_create(&thr, NULL, worker_thread, var);
		var->ids[i] = thr;
		pthread_detach(thr);
//		usleep(200);
		//printf("thr=%d\n", thr);
	}
	
//	sleep(10000);
//	pthread_join(thr, NULL);
	struct timeval end;
	struct timeval start;

	while (1)
	{
		i = -1;
		gettimeofday(&end, NULL);
		usleep(5000);
		pthread_mutex_lock(var->mtx);
		while (++i < n)	
		{
			start = var->start[i];
		//	printf("%ld start\n", start.tv_sec * 1000000 + start.tv_usec);
		//	printf("%ld end\n", (end.tv_sec * 1000000 + end.tv_usec));
	//		printf("#%d diff=%ld die=%d\n", i + 1, ((get_time(&end) - get_time(&start)) / 1000), var->die);
			if (start.tv_sec && start.tv_usec && ((get_time(&end) - get_time(&start)) / 1000 >= var->die))
			{
				printf("%ldms #%d %s\n", (get_time(&end) - get_time(&var->global_start)) / 1000, i + 1, "died");
				pthread_mutex_unlock(var->mtx);
				return (0);
			}
		}
		pthread_mutex_unlock(var->mtx);
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

	if (!(ac == 5 || ac == 6))
	{
		printf("error arg num\n");
		return (1);
	}
	var.die = ft_atoi(av[2]);
	var.eat = ft_atoi(av[3]);
	var.sleep = ft_atoi(av[4]);
	var.philos = ft_atoi(av[1]);
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
