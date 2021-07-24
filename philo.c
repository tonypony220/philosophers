#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#define PROTECT_PRINT 1
#define COLOR 1
#define DONE 1
#define JOINED 2
#define QUIT -1
#define RED 	"\033[1;31m"
#define GREEN   "\033[0;32m"
#define YELLOW	"\033[1;33m"
#define BLUE	"\033[1;34m"
#define PURPLE  "\033[1;35m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0;0m"
#define BOLD    "\033[;1m"
#define REVERSE "\033[;7m"

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
    pthread_mutex_t *mutexes;
    pthread_mutex_t *mtx;
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
};

long	get_time(struct timeval *time)
{
	return (time->tv_sec * 1000000 + time->tv_usec);
}

long	diff(struct timeval *end, struct timeval *start)
{
	return ((get_time(end) - get_time(start)) / 1000);
}
// (get_time(&current_time) - get_time(&data->global_start)) / 1000

int print_status(struct s_vars *data, char *status)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);

	PROTECT_PRINT && pthread_mutex_lock(data->mtx);
	!COLOR && printf("%ldms %d %s\n", 
			diff(&current_time, &data->global_start),
			data->num, status);
	COLOR && printf("%ldms \033[1;3%dm %d %s"RESET"\n",
			diff(&current_time, &data->global_start),
			data->num + 1,  data->num , status);
    PROTECT_PRINT && pthread_mutex_unlock(data->mtx);
	return (1);
}

void save_data_to_thread(struct s_vars *data, struct s_vars *data_2)
{
    data->mutexes		= data_2->mutexes;
	data->die			= data_2->die;
	data->eat			= data_2->eat;
	data->sleep			= data_2->sleep;
	data->meal_num		= data_2->meal_num;
	data->num			= data_2->num;
	data->mtx			= data_2->mtx;
	data->start			= data_2->start;
	data->global_start	= data_2->global_start;
	data->philos		= data_2->philos;
	data->status		= data_2->status;
}

/* i: philo number */
void *worker_thread(void *v_data)
{
	struct s_vars	data;	
	int				fork;
	int				fst;
	int 			sec;
	int				meals;
	struct timeval	end;

	save_data_to_thread(&data, v_data);
	gettimeofday(&data.start[data.num], NULL);
	pthread_mutex_unlock(data.mtx);
	// 0  0  1 
	// 1  1  2
	// 2  2  3
	// 3  3  4
	// 4  4  0
	fst = (data.num +  (data.num % 2)) % data.philos; // && usleep(200);
	sec = (data.num + !(data.num % 2)) % data.philos; // && usleep(200);
	
	///sec = !(fst) && 1;
	//data.num == 1 && (sec = 1) && (fst = 0);
	//fst = 0;
	//sec = 1;
//	printf("phil %d first %d, second %d\n", data.num, fst, sec);
	
	////////print_status(&data, "started\n");
	/////pthread_mutex_unlock(((struct s_vars *)v_data)->mtx);
	meals = 0;
	char *str = malloc(300);
	while (1)
	{
        pthread_mutex_lock(&data.mutexes[fst]);
		sprintf(str, "taken fork=%d  ", fst);
		print_status(&data, str);
		////print_status(&data, "has taken a fork");
        pthread_mutex_lock(&data.mutexes[sec]);
		sprintf(str, "taken fork=%d ", sec);
		////print_status(&data, "has taken a fork");
		print_status(&data, str);
		pthread_mutex_lock(data.mtx);
    	gettimeofday(&data.start[data.num], NULL);
		pthread_mutex_unlock(data.mtx);
		print_status(&data, "eating");
	 // 	gettimeofday(&data.start[data.num], NULL);
		usleep(data.eat * 1000);
    	pthread_mutex_unlock(&data.mutexes[sec]);
      	pthread_mutex_unlock(&data.mutexes[fst]);
	//	printf("released #%d fork=%d \n", data.num,  sec);
///		pthread_mutex_lock(data.mtx);
///    	gettimeofday(&data.start[data.num], NULL);
///		pthread_mutex_unlock(data.mtx);
		meals++;

		pthread_mutex_lock(data.mtx);
		(data.status[data.num] == QUIT) && (data.meal_num = QUIT);
		pthread_mutex_unlock(data.mtx);

		if ((data.meal_num && meals == data.meal_num) || data.meal_num == QUIT)
		{
			pthread_mutex_lock(data.mtx);
			data.status[data.num] = DONE;
			pthread_mutex_unlock(data.mtx);
			break ;
		}
	//	printf("released #%d fork=%d \n", data.num,  fst);
	//	gettimeofday(&end, NULL);
		print_status(&data, "is sleeping");
		usleep(data.sleep * 1000);
		print_status(&data, "is thinking");
		usleep(1000);


	//	while (1)
	//	{
	//		gettimeofday(&end, NULL);
	//		printf("%d %ld << \n", data.num + 1, (get_time(&end) - get_time(&data.start[data.num])) / 1000);
	//		if ((get_time(&end) - get_time(&data.start[data.num])) > data.die * (1000 >> 3))
	//		{
	//			break ;
	//		}
	//		//usleep(data.die / 8 * 1000);
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
//:w
//print_status(&data, "died");
    return (0);  /* never reached */
}

int exiting(struct s_vars *var, int n, int code)
{
	int i;

	i = -1;
	var->mtx && pthread_mutex_destroy(var->mtx);
	while (var->mutexes && ++i < n)	
		pthread_mutex_destroy(&var->mutexes[i]);
	if (var->mutexes)
		free(var->mutexes);
	if (var->mtx)
		free(var->mtx);
	if (var->ids)
		free(var->ids);
	if (var->start)
		free(var->start);
	if (var->status)
		free(var->status);
	(code == 1) && printf(RED"error malloc"RESET"\n");
	(code == 2) && printf(RED"error mutex init"RESET"\n");
	(code == 3) && printf(YELLOW"death"RESET"\n");
	printf("exiting\n");
	return (code);
}

int terminate_thread_signal(struct s_vars *var, int num)
{
	pthread_mutex_lock(var->mtx);
	var->status[num] = QUIT;
	pthread_mutex_unlock(var->mtx);
	return (1);
}

int tracking(struct s_vars *var, int n)
{
    pthread_t		thr;
	struct timeval	end;
	struct timeval	start;
	int				i;
	char			status;
	int				joined;
	char			exiting;

	exiting = 0;
	joined = 0;
	while (joined != n)
	{
		i = -1;
		usleep(9500);
		while (++i < n)	
		{
			gettimeofday(&end, NULL);
			pthread_mutex_lock(var->mtx);
			start = var->start[i];
			status = var->status[i];
			thr = var->ids[i];
			pthread_mutex_unlock(var->mtx);
		//	printf("%ld start\n", start.tv_sec * 1000000 + start.tv_usec);
		//	printf("%ld end\n", (end.tv_sec * 1000000 + end.tv_usec));
//			printf("#%d diff=%ld die=%d\n", i + 1, ((get_time(&end) - get_time(&start)) / 1000), var->die);
			exiting && terminate_thread_signal(var, i);
			if (!exiting && start.tv_sec && start.tv_usec && (diff(&end, &start) >= var->die))
			{
				printf("%ldms #%d %s\n", diff(&end, &var->global_start), i, "died");
				terminate_thread_signal(var, i);
				exiting = 1;
			}
			if (status == DONE)
			{
				pthread_join(thr, NULL);
				printf("%ldms #%d %s\n", diff(&end, &var->global_start), i, "JOINED");
				pthread_mutex_lock(var->mtx);
				var->status[i] = JOINED;
				pthread_mutex_unlock(var->mtx);
				joined++;
			}
		}
	}
	return (0);
}

int lunch(struct s_vars *var, int n)
{
    pthread_t	thr;
	int			i;

	//printf("size of pthread%ld %ld<<<<<:\n", sizeof(a), sizeof(pthread_mutex_t));

	var->mutexes = malloc(sizeof(pthread_mutex_t) * (n + 0));
	var->mtx = malloc(sizeof(pthread_mutex_t));
	var->ids = malloc(sizeof(pthread_t) * n);
	var->start = malloc(sizeof(struct timeval) * n);
	var->status = malloc(sizeof(char) * n);
	if (!var->mutexes || !var->mtx || !var->ids || !var->start || !var->status)
		return (exiting(var, n, 1));
	memset(var->mutexes, 0, sizeof(pthread_mutex_t) * (n + 0));
	memset(var->mtx, 0, sizeof(pthread_mutex_t));
	memset(var->ids, 0, sizeof(pthread_t) * n);
	memset(var->start, 0, sizeof(struct timeval) * n);
	memset(var->status, 0, sizeof(char) * n);
	if (pthread_mutex_init(var->mtx, NULL))
		return (exiting(var, n, 2));

	i = -1;
//	while (++i < n)
//		printf("%d thr", var->ids[i]);
	i = -1;
	gettimeofday(&var->global_start, NULL);
	while (++i < n)	
	{
		if (pthread_mutex_init(&var->mutexes[i], NULL))
			return (exiting(var, n, 2));
		pthread_mutex_lock(var->mtx);
		var->num = i;
		pthread_create(&thr, NULL, worker_thread, var);
		var->ids[i] = thr;
		pthread_detach(thr);
		//usleep(200);
		//printf("thr=%d\n", thr);
	}
	return (exiting(var, n, tracking(var, n)));	
//	sleep(10000);
//	pthread_join(thr, NULL);
	sleep(10);
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
	memset(&var, 0, sizeof(struct s_vars));
	var.die = ft_atoi(av[2]);
	var.eat = ft_atoi(av[3]);
	var.sleep = ft_atoi(av[4]);
	var.philos = ft_atoi(av[1]);
	var.meal_num = 0x0;
	ac == 6 && (var.meal_num = ft_atoi(av[5]));
	return (lunch(&var, ft_atoi(av[1])));
}

int main(int ac, char **av)
{
	return(_main(ac, av));
}
