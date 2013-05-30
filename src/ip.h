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
#ifndef ETHERTYPE_IPV6
#define ETHERTYPE_IPV6 0x86dd 
#endif
void get_rand_addr(
        struct sockaddr * sin_net,
        struct sockaddr * sin_smask,
        struct sockaddr * sin_addr);
void set_mask(struct sockaddr * sin,int family, int mask);
unsigned short int inet_cksum(unsigned short int *addr, size_t len);
int getipaddr(char * name, struct sockaddr * sin,int family);


#ifdef	__cplusplus
}
#endif

#endif	/* IP_H */

