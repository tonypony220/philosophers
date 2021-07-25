#include "philo.h"

int	ft_atoi(char *str)
{
	int		sign;
	long	num;

	num = 0;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	sign = 1 - 2 * (*str == '-' && 1);
	*str == '-' && str++;
	while (*str >= '0' && *str <= '9')
	{
		num = num * 10 + *str++ - '0';
		if (num < 0)
			return ((sign > 0) * -1);
	}
	return ((int)num * sign);
}

int	sleeping(long amount)
{
	usleep((long)(amount * MISALIGNMENT));
	return (1);
}

long	get_time(struct timeval *time)
{
	return (time->tv_sec * 1000000 + time->tv_usec);
}

long	diff(struct timeval *end, struct timeval *start)
{
	return ((get_time(end) - get_time(start)) / 1000);
}

int	print_status(struct s_vars *data, char *status, int i)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	PROTECT_PRINT && sem_wait(data->sem_disp);
	!COLOR && printf("%ldms %d %s\n",
			diff(&current_time, &data->global_start),
			i + HUMAN_NUM, status);
	COLOR && SHOW_STARVING && printf("%ldms %ldms \033[1;3%dm %d "RESET"%s\n",
			diff(&current_time, &data->global_start),
			diff(&current_time, &data->start),
			data->num + 1, data->num + HUMAN_NUM, status);
	COLOR && !SHOW_STARVING && printf("%ldms \033[1;3%dm %d "RESET"%s\n",
			diff(&current_time, &data->global_start),
			i + 1, i + HUMAN_NUM, status);
	PROTECT_PRINT && sem_post(data->sem_disp);
	return (1);
}
