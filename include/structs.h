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

typedef struct s_packet t_packet;

typedef enum e_protocols
{
	ICMP = IPPROTO_ICMP,
	TCP = IPPROTO_TCP,
	UDP = IPPROTO_UDP
} t_protocols;

typedef struct s_data
{
	t_args *args;
	struct addrinfo *infos;
	uint8_t done;

	int32_t sock;

#ifndef NO_SUDO
	int32_t sock_recv;
#endif

	size_t size; // size of the payload
	uint8_t max_hops;
	ssize_t pbh;  // probes per hop
	uint16_t pst; // Probes at the Same Time
	uint16_t *port;

	char ip[INET_ADDRSTRLEN];
	suseconds_t *rtt;

	t_packet *packets;

	t_protocols prot; // Every value beside those defined is invalid
} t_data;

typedef struct s_packet
{
	uint16_t ttl;

	union
	{
		struct icmphdr icmp;
		struct tcphdr tcp;
	};

	char payload[];
} t_packet;

#ifndef NO_SUDO

typedef struct s_packet_recv
{
	struct iphdr ip;
	struct icmphdr icmp;
	char payload[];
} t_recv_packet;

#endif

extern t_data *g_data;

#endif
