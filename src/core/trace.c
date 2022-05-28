#include "ft_traceroute.h"
#include "random.h"

#include <stdio.h>
#include <sys/select.h>

#include <errno.h>
#include <string.h>

int get_prot_id(t_protocols prot)
{
	switch (prot)
	{
	case UDP:
		return 0;
	case ICMP:
		return 1;
	case TCP:
		return 2;
	}

	return -1;
}

static void send_udp_probe(t_packet *packet)
{
	if (setsockopt(g_data->sock, IPPROTO_IP, IP_TTL, &packet->ttl, sizeof(uint16_t)) == -1)
		warn("setsockopt failed");

	int err = RECVERR;
	if (setsockopt(g_data->sock, SOL_IP, IP_RECVERR, &err, sizeof(err)) == -1)
		warn("setsockopt failed");

	if (sendto(g_data->sock, packet->payload, PAYLOAD_SIZE, 0,
			   g_data->infos->ai_addr, g_data->infos->ai_addrlen) == -1)
		warn("sendto failed");
	g_data->rtt[0] = ft_get_time();
}

static void recv_probe()
{
	struct timeval tv;
	t_recv_packet packet;
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	char ip[INET_ADDRSTRLEN];

	tv.tv_sec = TIMEOUT_SEC;
	tv.tv_usec = TIMEOUT_USEC;

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
			struct in_addr a = {.s_addr = packet.ip.saddr};
			ft_strcpy(ip, inet_ntoa(a));
			if (ft_strcmp(ip, g_data->ip))
			{
				ft_memcpy(g_data->ip, ip, INET_ADDRSTRLEN);
				dprintf(1, " %s", ip);
			}
			dprintf(1, "  %.3f ms", ft_get_diff(g_data->rtt[0]));

			a = ((struct sockaddr_in *)g_data->infos->ai_addr)->sin_addr;
			if (!ft_strcmp(ip, inet_ntoa(a)))
				g_data->done = 1;
		}
	}
	else
		dprintf(1, " *");
}

void __trace()
{
	int prot = get_prot_id(g_data->prot);
	void (*send_probe[3])(t_packet *);
	send_probe[0] = &send_udp_probe;
	// send_probe[1] = &send_tcp_probe;
	// send_probe[2] = &send_icmp_probe;

	for (int i = FIRST_TTL; i < g_data->max_hops && !g_data->done; i++)
	{
		dprintf(1, "%2d ", i);
		ft_memset(g_data->ip, 0, INET_ADDRSTRLEN);

		for (int j = 0; j < g_data->pbh; j++)
		{
			t_packet *packet = create_packet(i);
			send_probe[prot](packet);
			recv_probe();
			free(packet);
			(*g_data->port)++;
		}
		dprintf(1, "\n");
	}
}
