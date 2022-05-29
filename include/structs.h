#ifndef STRUCTS_H
#define STRUCTS_H

#include <stddef.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>

#include <sys/time.h>

#include "ft_getopt.h"
#include "defines.h"

typedef struct s_data
{
	t_args *args;
	struct addrinfo *infos;
	uint8_t done;

	char dst[INET_ADDRSTRLEN];
	struct sockaddr *dst_addr;
	size_t dst_size;

	int32_t sock_send;
	int32_t sock_recv;

	uint8_t max_hops;
	ssize_t pbh; // probes per hop
	ssize_t start;
	suseconds_t timeout;
	uint8_t proto;

	char ip[INET_ADDRSTRLEN];
	suseconds_t rtt;
} t_data;

typedef struct s_packet
{
	struct icmp icmp;
	char payload[PAYLOAD_SIZE];
} t_packet;

extern t_data *g_data;

#endif
