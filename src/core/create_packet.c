#include "ft_traceroute.h"
#include "random.h"

static void fill_icmp_hdr(t_packet *packet)
{
	struct icmphdr *icmp_hdr = &packet->icmp;

	icmp_hdr->type = ICMP_ECHO;
	icmp_hdr->code = 0;
	icmp_hdr->checksum = 0;
	icmp_hdr->un.echo.id = ft_htons(pcg64_range(0, 65535));
	icmp_hdr->un.echo.sequence = ft_htons(pcg64_range(0, 65535));

	icmp_hdr->checksum = ft_cksum((uint16_t *)icmp_hdr, sizeof(struct icmphdr));
}

static void fill_tcp_hdr(__unused t_packet *packet)
{
}

static void fill_probe(char *probe)
{
	int i = 0;

	while (i < PAYLOAD_SIZE)
		probe[i++] = pcg64_range(32, 127);
}

t_packet *create_packet(int ttl)
{
	t_packet *packet = malloc(sizeof(t_packet) + PAYLOAD_SIZE); // Needed by flexible payload
	void (*fill_hdr[3])(t_packet *);
	fill_hdr[0] = NULL;
	fill_hdr[1] = &fill_icmp_hdr;
	fill_hdr[2] = &fill_tcp_hdr;

	if (!packet)
		return NULL;
	ft_memset(packet, 0, sizeof(t_packet) + PAYLOAD_SIZE);
	packet->ttl = ttl;

	int id = get_prot_id(g_data->prot);
	if (id)
		fill_hdr[id](packet);
	fill_probe(packet->payload);

	return packet;
}
