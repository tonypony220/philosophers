#include "philo.h"

/// void	save_data_to_thread(struct s_vars *data, struct s_vars *data_2)
/// {
/// 	data->sem_forks = data_2->sem_forks;
/// 	data->sem_disp = data_2->sem_disp;	
/// 	data->sem_local = data_2->sem_local;	
/// 	data->die = data_2->die;
/// 	data->eat = data_2->eat;
/// 	data->sleep = data_2->sleep;
/// 	data->meal_num = data_2->meal_num;
/// 	data->num = data_2->num;
/// 	data->start = data_2->start;
/// 	data->global_start = data_2->global_start;
/// 	data->philos = data_2->philos;
/// 	data->status = data_2->status;
/// }

int	set_shared_status_array(struct s_vars *vars)
{
	struct s_vars	data;

	data = *vars;
    sem_wait(data.sem_disp);
	data.status = DONE;
	sem_post(data.sem_disp);
	return (1);
}

int sem_err(void)
{
	printf(RED"sem error"RESET);
	exit(1);
	return (1);
}


int	eating_loop_part(struct s_vars *vars)
{
	sem_wait(vars->sem_waiter) && sem_err();
    sem_wait(vars->sem_forks) && sem_err();
	!vars->status && print_status(vars, "has taken a fork", vars->num);
	//print_status(vars, "has taken a fork", vars->num);
    sem_wait(vars->sem_forks) && sem_err();
	sem_post(vars->sem_waiter) && sem_err();
	//print_status(vars, "has taken a fork", vars->num);
	!vars->status && print_status(vars, "has taken a fork", vars->num);
    sem_wait(vars->sem_disp);
	gettimeofday(&vars->start, NULL);
	sem_post(vars->sem_disp);
//	if (vars->status != DEAD)
//	{
////	print_status(vars, "eating", vars->num);
////	sleeping(vars->eat * 1000);
	!vars->status && print_status(vars, "eating", vars->num);
	!vars->status && sleeping(vars->eat * 1000);
//	}
	sem_post(vars->sem_forks) && sem_err();
	sem_post(vars->sem_forks) && sem_err();
	return (1);
}

/* if i malloc something sem should be down 
 * on exiting to prevent segfault by thread 
 * when access this memory */
int	run_loop(struct s_vars *vars)
{
	int				meals;
	pthread_t		thr;

	meals = 0;
	printf("process %d starting loop\n", vars->num);
	pthread_create(&thr, NULL, tracking, vars);
	pthread_detach(thr);
	gettimeofday(&vars->start, NULL);
	while (1)
	{
		eating_loop_part(vars) || (vars->status = QUIT);
//		printf("(%d) time %ld\n", data.num, get_time(&data.start));
		meals++;
		if (vars->status == DEAD)// && (vars->meal_num = QUIT);
			exit(1);
		if ((vars->meal_num && meals == vars->meal_num)) 
			break ;
		!vars->status && print_status(vars, "is sleeping", vars->num);
		sleeping(vars->sleep * 1000);
		!vars->status && print_status(vars, "is thinking", vars->num);
	}
	exit(0);
}
