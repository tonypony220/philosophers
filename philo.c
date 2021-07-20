
struct vars 
{
    pthread_mutex_t **mutex;
	die				int;
	eat				int;
	sleep			int;
	meal_num		int;
};


/* args:
 * 		1 number_of_philosophers
 * 		2 time_to_die 
 * 		3 time_to_eat
 * 		4 time_to_sleep 
 * 		5 number_of_times_each_philosopher_must_eat
 */
int lunch(struct vars *var, int n)
{
	while (n--)	
	{

	}
}

int p_main(int ac, char **av)
{
	struct vars var;

	var.die = av[2]
	var.eat = av[3]
	var.sleep = av[4]
	var.meal_num = 0;
	ac == 6 && (var.meal_num = av[5]);
		
	var.mutex = PTHREAD_MUTEX_INITIALIZER;
	return(_main(ac, av));
}

