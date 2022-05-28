#include "ft_traceroute.h"
#include <unistd.h>

static void ft_set_icmp_hdr(struct icmp *icmp)
{
	static uint16_t seq = 1;

	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = getpid();
	icmp->icmp_seq = seq++;
	icmp->icmp_cksum = 0;

	icmp->icmp_cksum = ft_cksum((uint16_t *)icmp, sizeof(struct icmp));
}

t_packet *create_packet(int ttl)
{
	t_packet *packet = malloc(sizeof(t_packet));

	if (!packet)
		return NULL;
	ft_memset(packet, 0, sizeof(t_packet));
	ft_set_icmp_hdr(&packet->icmp);

	if (setsockopt(g_data->sock_send, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
		throw_error("setsockopt failed"); // Critical error, should not happen
	packet->icmp.icmp_cksum = ft_cksum((uint16_t *)&packet->icmp, sizeof(struct icmphdr));

	ft_memcpy(packet->payload, PAYLOAD, PAYLOAD_SIZE);
	return packet;
}
