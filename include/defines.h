#ifndef DEFINES_H
#define DEFINES_H

#define OPT_H 0x01
#define OPT_N 0x02
#define OPT_I 0x04
#define OPT_T 0x10

#define __unused __attribute((unused))

#define GET_TIME(rtt) (rtt.tv_sec * 1000000 + rtt.tv_usec)
#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x < y ? y : x)
#define ABS(x) (x < 0 ? -x : x)

#define FIRST_TTL 1
#define MAX_HOPS 30
#define PAYLOAD_SIZE 32
#define MAX_PACKETS 3
#define TIMEOUT_SEC 5
#define TIMEOUT_USEC 0
#define PROBE_BY_HOP 3
#define PROBES_SAME_TIME 1 // 16

#ifdef NO_SUDO
#define RECVERR 1
#else
#define RECVERR 0
#endif

#endif
