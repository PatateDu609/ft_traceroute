#include "ft_traceroute.h"
#include <stdio.h>
#include <unistd.h>

static int32_t ft_socket_recv()
{
	int32_t sock;

	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (sock == -1) // If no sudo should at least try to encapsulate ICMP in UDP
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);

	if (sock == -1)
		throw_error("socket failed");
	return sock;
}

void setup()
{
	g_data->sock_recv = ft_socket_recv();

	if (g_data->args->flags & OPT_I)
		g_data->sock_send = g_data->sock_recv;
	else
		g_data->sock_send = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	char *dst = inet_ntoa(((struct sockaddr_in *)g_data->dst_addr)->sin_addr);
	ft_memcpy(g_data->dst, dst, ft_strlen(dst));

	printf("ft_traceroute to %s (%s), %d hops max\n",
		   g_data->infos->ai_canonname, g_data->dst, g_data->max_hops);
}
