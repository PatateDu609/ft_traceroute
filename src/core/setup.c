#include "ft_traceroute.h"
#include <stdio.h>
#include <unistd.h>

static int32_t ft_socket()
{
	int32_t sock;

	switch (g_data->prot)
	{
	case ICMP:
		sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
		break;
	case TCP:
		if (getuid())
			dprintf(2, "You do not have enough privileges to use this traceroute method.");
		sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
		break;
	case UDP:
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		break;
	}

	struct sockaddr_in addr;
	ft_memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = ft_htons(0);
	inet_pton(addr.sin_family, "0.0.0.0", &addr.sin_addr);

	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		throw_error("bind failed");
	int mtu = 0;
	if (setsockopt(sock, SOL_IP, IP_MTU_DISCOVER, &mtu, sizeof(mtu)) == -1)
		warn("setsockopt failed");
	int old_timestamp = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_TIMESTAMP_OLD, &old_timestamp, sizeof(old_timestamp)) == -1)
		warn("setsockopt failed");
	int recv_ttl = 1;
	if (setsockopt(sock, IPPROTO_IP, IP_RECVTTL, &recv_ttl, sizeof(recv_ttl)) == -1)
		warn("setsockopt failed");

	if (sock == -1)
		throw_error("socket failed");
	return sock;
}

void setup()
{
	g_data->sock = ft_socket();

#ifndef NO_SUDO
	g_data->sock_recv = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_data->sock_recv == -1)
		throw_error("The creation of the socket receiver failed");
#endif

	printf("ft_traceroute to %s (%s), %d hops max, %ld byte packets\n",
		   g_data->infos->ai_canonname,
		   inet_ntoa(((struct sockaddr_in *)g_data->infos->ai_addr)->sin_addr),
		   g_data->max_hops,
		   g_data->size);
}
