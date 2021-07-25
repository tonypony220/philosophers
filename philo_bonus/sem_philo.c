#include "philo.h"

int create_semaphore(sem_t **sem, int val, char *name)
{
    *sem = sem_open(name, O_CREAT | O_EXCL, 0644, val); 
	if (*sem == SEM_FAILED)
	{
 		if (errno != EEXIST)
			return (0);
		sem_unlink(name);   
		sem_close(*sem);  
		*sem = sem_open(name, O_CREAT | O_EXCL, 0644, val); 
	}
	return (1);
}

int	waiting(struct s_vars *var)
{
	/* wait for all children to exit */
	int status;
	pid_t pid;

	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (status)
		{
			kill_all(var->pids);		
			break;
		}
		if (errno == ECHILD)
		{
			printf("exited");
			break;
		}
	}
	return (exiting(var, 0));
}

int initialize(struct s_vars *var, int n)
{
	var->pids = malloc(sizeof(pid_t) * (n + 1));
	var->pids || exiting(var, ERR_MALLOC);
	memset(var->pids, 0, sizeof(pid_t) * n);
	if (!create_semaphore(&var->sem_forks, var->philos, SEM_FORKS)
	 || !create_semaphore(&var->sem_disp, 1, SEM_DISPLAY)
	 || !create_semaphore(&var->sem_waiter, 1, SEM_WAITER))
		exiting(var, ERR_SEM);
	gettimeofday(&var->global_start, NULL);
	return (1);
}

int	lunch(struct s_vars *var, int n)
{
    int i;
	pid_t pid;
	
	initialize(var, n);
	i = -1;
    while (++i < n)
	{
        pid = fork();
        if (pid < 0) 
			exiting(var, ERR_FORK);
        else if (pid == 0)
            break;
		else
			var->pids[i] = pid;	
    }
    if (pid != 0)
		waiting(var);
    else
	{
		free(var->pids);
		var->num = i;
		run_loop(var);
    }
	return (1);
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
		return (exiting(&var, 4));
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
		return (exiting(&var, 4));
	return (lunch(&var, ft_atoi(av[1])));
}
