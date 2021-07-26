#include "philo.h"

int	getting_thread_info(struct s_vars *var, struct s_track *scope, int i)
{
	pthread_mutex_lock(var->mtx);
	scope->status = var->status[i];
	scope->thr = var->ids[i];
	pthread_mutex_unlock(var->mtx);
	return (1);
}

int	terminate_thread_signal(struct s_vars *var, int num, int sig)
{
	pthread_mutex_lock(var->mtx);
	var->status[num] = sig;
	pthread_mutex_unlock(var->mtx);
	return (1);
}

int	waiting_thread(struct s_vars *var, struct s_track *scope, int i)
{
	pthread_join(scope->thr, NULL);
	pthread_mutex_lock(var->mtx);
	var->status[i] = JOINED;
	pthread_mutex_unlock(var->mtx);
	scope->joined++;
	return (1);
}

int is_dead(struct s_vars *var, int i)
{
	struct timeval	current_time;
	int				res;

	gettimeofday(&current_time, NULL);
	pthread_mutex_lock(var->mtx);
	res = var->start[i].tv_sec && var->start[i].tv_usec 
		&& diff(&current_time, &var->start[i]) >= var->die;
	pthread_mutex_unlock(var->mtx);
	return (res);
}

int	tracking(struct s_vars *var, int n)
{
	struct s_track			scope;
	int						i;

	memset(&scope, 0, sizeof(struct s_track));
	while (scope.joined != n && scope.exiting < 400)
	{
		(i = -1) && usleep(4000);
		while (++i < n)
		{
			getting_thread_info(var, &scope, i);
			scope.exiting && terminate_thread_signal(var, i, QUIT);
			scope.exiting && ++scope.exiting;
			if (!scope.exiting && is_dead(var, i))
			{
				print_status(var, "died", i);
				terminate_thread_signal(var, i, DEAD);
				scope.exiting = 1;
			}
			if (scope.status == DONE)
				waiting_thread(var, &scope, i);
		}
	}
	return (0);
}
