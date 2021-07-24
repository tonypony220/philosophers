#include "philo.h"

int	allocate_mem(struct s_vars *var, int n)
{
	var->mutexes = malloc(sizeof(pthread_mutex_t) * (n));
	var->mtx = malloc(sizeof(pthread_mutex_t));
	var->ids = malloc(sizeof(pthread_t) * n);
	var->start = malloc(sizeof(struct timeval) * n);
	var->status = malloc(sizeof(char) * n);
	return (1);
}

int	null_memory(struct s_vars *var, int n)
{
	memset(var->mutexes, 0, sizeof(pthread_mutex_t) * n);
	memset(var->mtx, 0, sizeof(pthread_mutex_t));
	memset(var->ids, 0, sizeof(pthread_t) * n);
	memset(var->start, 0, sizeof(struct timeval) * n);
	memset(var->status, 0, sizeof(char) * n);
	return (1);
}

int	lunch(struct s_vars *var, int n)
{
	pthread_t	thr;
	int			i;

	allocate_mem(var, n);
	if (!var->mutexes || !var->mtx || !var->ids || !var->start || !var->status)
		return (exiting(var, n, 1));
	null_memory(var, n);
	if (pthread_mutex_init(var->mtx, NULL))
		return (exiting(var, n, 2));
	i = -1;
	while (++i < n)
		if (pthread_mutex_init(&var->mutexes[i], NULL))
			return (exiting(var, n, 2));
	i = -1;
	gettimeofday(&var->global_start, NULL);
	while (++i < n)
	{
		pthread_mutex_lock(var->mtx);
		var->num = i;
		pthread_create(&thr, NULL, worker_thread, var);
		var->ids[i] = thr;
		pthread_detach(thr);
	}
	return (exiting(var, n, tracking(var, n)));
}

/* args:
 * 		1 number_of_philosophers
 * 		2 time_to_die 
 * 		3 time_to_eat
 * 		4 time_to_sleep 
 * 		5 number_of_times_each_philosopher_must_eat
 */
int	main(int ac, char **av)
{
	struct s_vars	var;

	memset(&var, 0, sizeof(struct s_vars));
	if (!(ac == 5 || ac == 6))
		return (exiting(&var, 0, 4));
	var.die = ft_atoi(av[2]);
	var.eat = ft_atoi(av[3]);
	var.sleep = ft_atoi(av[4]);
	var.philos = ft_atoi(av[1]);
	var.meal_num = 0x0;
	ac == 6 && (var.meal_num = ft_atoi(av[5]));
	if (var.die <= 0
		|| var.eat <= 0
		|| var.sleep <= 0
		|| var.philos <= 0
		|| var.meal_num < 0)
		return (exiting(&var, 0, 4));
	return (lunch(&var, ft_atoi(av[1])));
}
