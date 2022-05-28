#include "ft_traceroute.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/udp.h>

t_data *g_data = NULL;

static void ft_resolve(char *arg)
{
	struct addrinfo hints;
	struct addrinfo *infos = NULL;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_CANONNAME | AI_ADDRCONFIG;
	hints.ai_socktype = 0;
	hints.ai_protocol = 0;

	if (getaddrinfo(arg, NULL, &hints, &infos) != 0)
		throw_error("getaddrinfo failed");

	g_data->infos = infos;
}

void trace(t_args *args)
{
	g_data = malloc(sizeof(t_data));

	if (!g_data)
		throw_error("malloc");
	ft_memset(g_data, 0, sizeof(t_data));
	g_data->args = args;
	ft_resolve(args->args[0]);
	g_data->max_hops = MAX_HOPS;
	g_data->pbh = PROBE_BY_HOP;

	struct sockaddr_in *dst_addr = ((struct sockaddr_in *)g_data->infos->ai_addr);
	dst_addr->sin_port = ft_htons(PORT);
	g_data->dst_addr = (struct sockaddr *)dst_addr;
	g_data->dst_size = sizeof(struct sockaddr_in);

	setup();
	__trace();
}
