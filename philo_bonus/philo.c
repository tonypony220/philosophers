#include "philo.h"



/* args:
 * 		1 number_of_philosophers
 * 		2 time_to_die 
 * 		3 time_to_eat
 * 		4 time_to_sleep 
 * 		5 number_of_times_each_philosopher_must_eat
 */
int	__main(int ac, char **av)
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
