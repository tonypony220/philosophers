#include "philo.h"

/* all process will wait */
int	getting_info(struct s_vars *var, struct s_track *scope)
{
	sem_wait(var->sem_disp);
	scope->start = var->start;
	scope->status = var->status;
	sem_post(var->sem_disp);
	return (1);
}

int	terminate_thread_signal(struct s_vars *var, int sig)
{
	sem_wait(var->sem_disp);
	var->status = sig;
	sem_post(var->sem_disp);
	return (1);
}

void	*tracking(void *v_data)
{
	struct s_track			scope;
	struct s_vars			*var;
	int						i;

	var = (struct s_vars *)v_data;
	memset(&scope, 0, sizeof(struct s_track));
	while (1)
	{
		gettimeofday(&scope.end, NULL);
		(i = -1) && usleep(3000);
		scope.exiting && terminate_thread_signal(var, QUIT);
		sem_wait(var->sem_disp);
		if (!scope.exiting && (diff(&scope.end, &var->start) >= var->die))
		{
			var->status = DEAD;
			sem_post(var->sem_disp);
			print_status(var, RED"died"RESET, var->num);
			sem_wait(var->sem_disp);
			exit(1);
			scope.exiting = 1;
			break ;
		}
		else
			sem_post(var->sem_disp);
	}
	return (NULL);
}
