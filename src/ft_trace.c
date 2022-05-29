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

static char *get_option_value(uint64_t flag)
{
	if (!(g_data->args->flags & flag))
		return NULL;

	for (int i = 0; i < g_data->args->nb_opt; i++)
		if (g_data->args->options[i].flag == flag)
			return g_data->args->options[i].value;
	return NULL;
}

static void setup_max_hops()
{
	char *value = get_option_value(OPT_m);
	if (value)
		g_data->max_hops = ft_atoi(value);
	else
		g_data->max_hops = MAX_HOPS;
}

static void setup_pbh()
{
	char *value = get_option_value(OPT_q);
	if (value)
		g_data->pbh = ft_atoi(value);
	else
		g_data->pbh = PROBES_BY_HOP;
}

static void setup_timeout()
{
	char *value = get_option_value(OPT_w);
	if (value)
		g_data->timeout = ft_atoi(value);
	else
		g_data->timeout = TIMEOUT_SEC;
}

static void setup_start()
{
	char *value = get_option_value(OPT_f);
	if (value)
		g_data->start = ft_atoi(value);
	else
		g_data->start = FIRST_TTL;
}

static void setup_proto()
{
	char *value = get_option_value(OPT_M);
	if (!value)
		g_data->proto = IPPROTO_UDP;
	else if (ft_strcmp("icmp", value))
		g_data->proto = IPPROTO_ICMP;
	else if (ft_strcmp("udp", value))
		g_data->proto = IPPROTO_UDP;
	else if (g_data->args->flags & OPT_I)
		g_data->proto = IPPROTO_ICMP;
}

void trace(t_args *args)
{
	g_data = malloc(sizeof(t_data));

	if (!g_data)
		throw_error("malloc");
	ft_memset(g_data, 0, sizeof(t_data));
	g_data->args = args;
	ft_resolve(args->args[0]);
	setup_max_hops();
	setup_pbh();
	setup_timeout();
	setup_start();
	setup_proto();

	struct sockaddr_in *dst_addr = ((struct sockaddr_in *)g_data->infos->ai_addr);
	dst_addr->sin_port = ft_htons(PORT);
	g_data->dst_addr = (struct sockaddr *)dst_addr;
	g_data->dst_size = sizeof(struct sockaddr_in);

	setup();
	__trace();
}
