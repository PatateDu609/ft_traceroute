#include "ft_traceroute.h"

#include <stdio.h>
#include <sys/select.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>

static void send_probe(__unused t_packet *packet)
{
	struct sockaddr addr;
	size_t size = g_data->dst_size;

	ft_memcpy(&addr, g_data->dst_addr, size);

	if (sendto(g_data->sock_send, PAYLOAD, PAYLOAD_SIZE, 0, &addr, size) == -1)
		warn("sendto failed");
	g_data->rtt = ft_get_time();
}

static void ft_get_print_addr(char *ip, struct sockaddr_in addr)
{
	if (g_data->args->flags & OPT_n)
	{
		dprintf(1, "  %s", ip);
		return;
	}

	char name[NI_MAXHOST];
	int ret = getnameinfo((struct sockaddr *)&addr, sizeof(addr), name, sizeof(name), NULL, 0, 0);
	if (ret)
		dprintf(1, "  %s", ip);
	else
		dprintf(1, "  %s (%s)", ip, name);
}

static void recv_probe()
{
	t_packet packet;
	struct timeval tv;
	char ip[INET_ADDRSTRLEN];
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	tv.tv_sec = g_data->timeout / 1000000;
	tv.tv_usec = g_data->timeout % 1000000;

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(g_data->sock_recv, &readfds);

	int ret = select(g_data->sock_recv + 1, &readfds, NULL, NULL, &tv);
	if (ret == -1)
		warn("select failed");
	else if (ret && FD_ISSET(g_data->sock_recv, &readfds))
	{
		if (recvfrom(g_data->sock_recv, &packet, sizeof(packet), 0,
					 (struct sockaddr *)&addr, &addrlen) == -1)
			warn("recvfrom failed");
		else
		{
			float diff = ft_get_diff(g_data->rtt);

			ft_strcpy(ip, inet_ntoa(addr.sin_addr));
			if (ft_strcmp(ip, g_data->ip))
			{
				ft_memcpy(g_data->ip, ip, INET_ADDRSTRLEN);
				ft_get_print_addr(ip, addr);
			}
			dprintf(1, "  %.3f ms", diff);

			if (!ft_strcmp(ip, g_data->dst))
				g_data->done = 1;
		}
	}
	else
		dprintf(1, "  *");
}

void __trace()
{
	for (int ttl = g_data->start; ttl <= g_data->max_hops && !g_data->done; ttl++)
	{
		dprintf(1, "%2d", ttl);
		ft_memset(g_data->ip, 0, INET_ADDRSTRLEN);

		if (setsockopt(g_data->sock_send, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
			throw_error("setsockopt failed"); // Critical error, should never happen

		for (int j = 0; j < g_data->pbh; j++)
		{
			send_probe(NULL);
			recv_probe();
		}
		dprintf(1, "\n");

		struct sockaddr_in *addr = (struct sockaddr_in *)g_data->dst_addr;
		addr->sin_port = ft_htons(ft_ntohs(addr->sin_port) + 1);
	}
}
