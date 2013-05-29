/* 
 * File:   ip.h
 * Author: targuan
 *
 * Created on 29 mai 2013, 20:18
 */

#ifndef IP_H
#define	IP_H

#ifdef	__cplusplus
extern "C" {
#endif

int getipaddr(char * name, struct sockaddr * sin,int family);


#ifdef	__cplusplus
}
#endif

#endif	/* IP_H */

