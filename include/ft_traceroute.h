#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include "ft_getopt.h"

#include "structs.h"
#include "defines.h"

void setup();
void __trace();
void trace(t_args *args);

void throw_error(char *msg);
void warn(char *msg);

uint16_t ft_cksum(uint16_t *addr, int len);

uint16_t ft_htons(uint16_t hostshort);
uint32_t ft_htonl(uint32_t hostlong);
uint16_t ft_ntohs(uint16_t netshort);
uint32_t ft_ntohl(uint32_t netlong);

t_packet *create_packet(int ttl);
suseconds_t ft_get_time(void);
float ft_get_diff(suseconds_t start);

#endif
