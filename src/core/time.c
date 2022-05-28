#include "ft_traceroute.h"
#include <sys/time.h>

suseconds_t ft_get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

float ft_get_diff(suseconds_t start)
{
	suseconds_t end = ft_get_time();
	return (end - start) / 1000.0f;
}
