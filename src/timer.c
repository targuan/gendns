#include "timer.h"

void set_ts(struct timeval * ts, int packet, struct options_args *options)
{
    ts->tv_usec = packet;
}