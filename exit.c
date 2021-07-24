#include "philo.h"

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
	(code == 1) && printf(RED"error: malloc"RESET"\n");
	(code == 2) && printf(RED"error: mutex init"RESET"\n");
	(code == 3) && printf(YELLOW"death"RESET"\n");
	(code == 4) && printf(RED"error: argument"RESET"\n");
	printf("exiting\n");
	return (code && 1);
}

