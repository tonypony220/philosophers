#include "philo.h"

int getting_thread_info(struct s_vars *var, struct s_track *scope, int i)
{
	pthread_mutex_lock(var->mtx);
	scope->start = var->start[i];
	scope->status = var->status[i];
	scope->thr = var->ids[i];
	pthread_mutex_unlock(var->mtx);
	return (1);
}

int terminate_thread_signal(struct s_vars *var, int num, int sig)
{
	pthread_mutex_lock(var->mtx);
	var->status[num] = sig;
	pthread_mutex_unlock(var->mtx);
	return (1);
}

int waiting_thread(struct s_vars *var, struct s_track *scope, int i)
{
	pthread_join(scope->thr, NULL);
	pthread_mutex_lock(var->mtx);
	var->status[i] = JOINED;
	pthread_mutex_unlock(var->mtx);
	scope->joined++;
	return (1);
}

				//printf("%ldms #%d %s\n", diff(&end, &var->global_start), i, "died");
			//	COLOR && printf("%ldms %ldfrom meal \033[1;3%dm %d %s"RESET"\n",
			//			diff(&end, &var->global_start),
			//			diff(&end, &var->start[i]),
			//			i + 1, i + HUMAN_NUM, "died");
	// printing joined threads;	
	//			printf("%ldms #%d %s\n", 
	//					diff(&end, &var->global_start),
	//					i + HUMAN_NUM, BOLD"terminated"RESET);
			
int tracking(struct s_vars *var, int n)
{
	struct s_track			scope;
	int						i;

	memset(&scope, 0, sizeof(struct s_track));
	while (scope.joined != n)
	{
		(i = -1) && usleep(4000);
		while (++i < n)	
		{
			gettimeofday(&scope.end, NULL);
			getting_thread_info(var, &scope, i);	
			scope.exiting && terminate_thread_signal(var, i, QUIT);
			if (!scope.exiting && scope.start.tv_sec && scope.start.tv_usec 
				&& (diff(&scope.end, &scope.start) >= var->die))
			{
				terminate_thread_signal(var, i, DEAD);
				print_status(var, "died", i);
				scope.exiting = 1;
			}
			if (scope.status == DONE)
				waiting_thread(var, &scope, i);
		}
	}
	return (0);
}
