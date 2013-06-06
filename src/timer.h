/* 
 * File:   timer.h
 * Author: targuan
 *
 * Created on 6 juin 2013, 19:34
 */

#ifndef TIMER_H
#define	TIMER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <sys/time.h>
#include "options.h"
    void set_ts(struct timeval * ts, int packet, struct options_args *options);


#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */

