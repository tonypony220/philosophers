#include <sys/time.h>

int	gettimeofday(struct timeval *restrict tp, void *restrict tzp);

int main()
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);

  	printf("seconds : %ld\nmicro seconds : %ld", 
			current_time.tv_sec * 1000000, current_time.tv_usec);
}
