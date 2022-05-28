#include "ft_traceroute.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/udp.h>
#include "random.h"

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
	{
		perror("getaddrinfo");
		exit(1);
	}

	g_data->infos = infos;
	g_data->port = &((struct sockaddr_in *)infos->ai_addr)->sin_port;
}

static size_t ft_sizeof_hdr(void)
{
	size_t size = 0;

	switch (g_data->prot)
	{
	case ICMP:
		size = sizeof(struct icmphdr);
		break;
	case TCP:
		size = sizeof(struct tcphdr);
		break;
	case UDP:
		size = sizeof(struct udphdr);
		break;
	}
	return size;
}

static t_protocols ft_get_prot(void)
{
	t_protocols prot = UDP;

	if (g_data->args->flags & OPT_I)
		prot = ICMP;
	else if (g_data->args->flags & OPT_T)
		prot = TCP;

	return prot;
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
	g_data->pst = PROBES_SAME_TIME;
	g_data->prot = ft_get_prot();
	*g_data->port = pcg64_range(UINT16_MAX / 2, UINT16_MAX);
	g_data->size = PAYLOAD_SIZE + sizeof(struct iphdr) + ft_sizeof_hdr();
	g_data->rtt = malloc(sizeof(suseconds_t) * g_data->pst);
	if (!g_data->rtt)
		throw_error("malloc");
	ft_memset(g_data->rtt, 0, sizeof(suseconds_t) * g_data->pst);

	if (g_data->prot == ICMP && getuid())
		g_data->prot = UDP;

	setup();
	__trace();
}
