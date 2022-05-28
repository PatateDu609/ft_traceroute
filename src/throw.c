#include "ft_traceroute.h"

#include <stdio.h>

#ifdef DEBUG
#include <errno.h>
#include <string.h>
#endif

void throw_error(char *msg)
{
#ifndef DEBUG
	fprintf(stderr, "ft_traceroute: %s\n", msg);
#else
	fprintf(stderr, "ft_traceroute: %s: %s\n", msg, strerror(errno));
#endif
	exit(1);
}

void warn(char *msg)
{
#ifndef DEBUG
	fprintf(stderr, "ft_traceroute: %s\n", msg);
#else
	fprintf(stderr, "ft_traceroute: %s: %s\n", msg, strerror(errno));
#endif
}
