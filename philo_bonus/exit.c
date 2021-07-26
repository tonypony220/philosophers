#include "philo.h"

int kill_all(pid_t *pids, int n)
{
	int ret;

	while (n--)
	{
		pids[n] && (ret = kill(pids[n], SIGINT));
		DEBUG && printf("kill process pid %d ret=%d\n", pids[n], ret);
	}
	return (1);
}

int close_semaphore(sem_t *sem, char *name)
{
	sem_unlink(name) && printf(RED"%s sem unlink failure\n", name);   
	sem_close(sem) && printf(RED"%s sem close failure\n", name);  
	return (1);
}

/* note: that if processes using semafore not 
 * has been terminated,
 * closeing semafore will return error */
int	exiting(struct s_vars *var, int code)
{
	var->pids && kill_all(var->pids, var->philos);
	if (var->pids)
		free(var->pids);
	var->sem_forks && close_semaphore(var->sem_forks, SEM_FORKS);
	var->sem_disp && close_semaphore(var->sem_disp, SEM_DISPLAY);
	var->sem_waiter && close_semaphore(var->sem_waiter, SEM_WAITER);
	(code == ERR_MALLOC) && printf(RED"error: malloc"RESET"\n");
	(code == ERR_SEM) && printf(RED"error: error semafore"RESET"\n");
	(code == ERR_FORK) && printf(RED"error: fork err"RESET"\n");
	(code == ERR_ARG) && printf(RED"error: argument"RESET"\n");
	exit(code && 1);
	return (0);
}
