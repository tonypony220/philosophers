#include "philo.h"

int kill_all(pid_t *pids)
{
	while (*pids)
		kill(*pids++, SIGINT);	
	return (1);
}

int close_semaphore(sem_t *sem, char *name)
{
	sem_unlink (name) || printf(RED"%s sem unlink failure\n", name);   
	printf("%s\n", strerror(errno));
	sem_close(sem) || printf(RED"%s sem close failure\n", name);  
	printf("%s\n", strerror(errno));
	return (1);
}

int	exiting(struct s_vars *var, int code)
{
	var->pids && kill_all(var->pids);
	if (var->pids)
		free(var->pids);
	var->sem_forks && close_semaphore(var->sem_forks, SEM_FORKS);
	var->sem_disp && close_semaphore(var->sem_disp, SEM_DISPLAY);
	var->sem_waiter && close_semaphore(var->sem_waiter, SEM_WAITER);
	(code == ERR_MALLOC) && printf(RED"error: malloc"RESET"\n");
	(code == ERR_SEM) && printf(RED"error: error semafore"RESET"\n");
	(code == ERR_FORK) && printf(RED"error: fork err"RESET"\n");
	(code == ERR_ARG) && printf(RED"error: argument"RESET"\n");
	printf("exiting\n");
	exit(code && 1);
	return (0);
}
