#ifndef DEFINES_H
#define DEFINES_H

#define OPT_H 0x01
#define OPT_N 0x02
#define OPT_I 0x04
#define OPT_T 0x10

#define __unused __attribute((unused))

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x < y ? y : x)
#define ABS(x) (x < 0 ? -x : x)

#define FIRST_TTL 1
#define MAX_HOPS 64
#define PAYLOAD "SUPERMAN"
#define PAYLOAD_SIZE 9
#define MAX_PACKETS 3
#define TIMEOUT_SEC 3
#define TIMEOUT_USEC 0
#define PROBE_BY_HOP 3

#define PORT 33434

#endif
