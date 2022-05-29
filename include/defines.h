#ifndef DEFINES_H
#define DEFINES_H

#define OPT_H 0x01
#define OPT_n 0x02
#define OPT_I 0x04
#define OPT_q 0x08
#define OPT_M 0x10
#define OPT_m 0x20
#define OPT_w 0x40
#define OPT_p 0x80
#define OPT_f 0x100

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
#define PROBES_BY_HOP 3

#define PORT 33434

#endif
