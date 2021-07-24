#include "philo.h"

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

int set_shared_status_array(struct s_vars *vars)
{
	struct s_vars	data;	
	
	data = *vars;
	pthread_mutex_lock(data.mtx);
	data.status[data.num] = DONE;
	pthread_mutex_unlock(data.mtx);
	return (1);	
}

int eating_loop_part(struct s_vars *vars)
{
	struct s_vars	data;	
	char 			status;

	data = *vars;
	(data.num % 2) && usleep(1000);
    pthread_mutex_lock(&data.mutexes[data.fst]);
	print_status(&data, "has taken a fork", data.num);
    pthread_mutex_lock(&data.mutexes[data.sec]);
	pthread_mutex_lock(data.mtx);
    gettimeofday(&data.start[data.num], NULL);
	status = data.status[data.num];
	pthread_mutex_unlock(data.mtx);
	if (status != DEAD)
	{
		print_status(&data, "eating", data.num);
		sleeping(data.eat * 1000);
	}
    pthread_mutex_unlock(&data.mutexes[data.sec]);
    pthread_mutex_unlock(&data.mutexes[data.fst]);
	return (1);
}

void	*run_loop(struct s_vars *vars)
{
	int				meals;
	struct s_vars	data;	
	
	meals = 0;
	data = *vars;
	while (1)
	{
		eating_loop_part(&data);
		meals++;
		pthread_mutex_lock(data.mtx);
		(data.status[data.num] == QUIT) && (data.meal_num = QUIT);
		pthread_mutex_unlock(data.mtx);
		if (((data.meal_num && meals == data.meal_num) || data.meal_num == QUIT) 
				&& set_shared_status_array(&data))
			break ;
		print_status(&data, "is sleeping", data.num);
		sleeping(data.sleep * 1000);
		print_status(&data, "is thinking", data.num);
		(data.philos % 2) && usleep(5000);
	}
	return (0);
}

void *worker_thread(void *v_data)
{
	struct s_vars	data;	

	save_data_to_thread(&data, v_data);
	gettimeofday(&data.start[data.num], NULL);
	pthread_mutex_unlock(data.mtx);
	data.fst = (data.num +  (data.num % 2)) % data.philos; // && usleep(200);
	data.sec = (data.num + !(data.num % 2)) % data.philos; // && usleep(200);
	printf("forks of philosopher %d left=%d right=%d \n", data.num + HUMAN_NUM, data.fst, data.sec);
	return (run_loop(&data));
}
