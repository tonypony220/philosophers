
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
	die				int;
	eat				int;
	sleep			int;
	meal_num		int;
};

/* i: philo number */
void *worker_thread(void *v_data)
{
	while ()
	{
        pthread_mutex_lock(mutexes);
        pthread_mutex_unlock(mutexes);
    }
    return NULL;  /* never reached */
}


int lunch(struct s_vars *var, int n)
{
	var->mutexes = malloc(sizeof(pthread_mutex_t) * (n + 1));
	memset(var->mutexes, 0, sizeof(pthread_mutex_t) * (n + 1));
	while (n--)	
	{
		var->mutexes[n] = PTHREAD_MUTEX_INITIALIZER;
	}
}

/* args:
 * 		1 number_of_philosophers
 * 		2 time_to_die 
 * 		3 time_to_eat
 * 		4 time_to_sleep 
 * 		5 number_of_times_each_philosopher_must_eat
 */
int p_main(int ac, char **av)
{
	struct s_vars var;

	var.die = ft_atoi(av[2]);
	var.eat = ft_atoi(av[3]);
	var.sleep = ft_atoi(av[4]);
	var.meal_num = 0;
	ac == 6 && (var.meal_num = ft_atoi(av[5]));
	lunch(&var, ft_atoi(av[1]));	
	return(_main(ac, av));
}
