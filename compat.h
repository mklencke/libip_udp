#ifndef __COMPAT_H
#define __COMPAT_H

#include <sys/types.h>

typedef u_int8_t u8_t;
typedef u_int16_t u16_t;
typedef u_int32_t u32_t;

typedef void (*sighandler_t)(int);
sighandler_t sysv_signal(int signum, sighandler_t handler);
#define signal sysv_signal

#endif

