#include <time.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "ip.h"

void get_rand_addr(
        struct sockaddr * sin_net,
        struct sockaddr * sin_smask,
        struct sockaddr * sin_addr) {
    
    struct sockaddr_in6 * sin_net_6 = sin_net;
    struct sockaddr_in * sin_net_4 = sin_net;
    
    struct sockaddr_in6 * sin_smask_6 = sin_smask;
    struct sockaddr_in * sin_smask_4 = sin_smask;
    
    struct sockaddr_in6 * sin_addr_6 = sin_addr;
    struct sockaddr_in * sin_addr_4 = sin_addr;
    
    uint32_t r;
    
    int i;
    
    sin_addr->sa_family = sin_net->sa_family;
    
    if(sin_net->sa_family == AF_INET) {
        r = rand();

        sin_addr_4->sin_addr.s_addr =  (r&(~sin_smask_4->sin_addr.s_addr)) | 
                (sin_net_4->sin_addr.s_addr & sin_smask_4->sin_addr.s_addr);
        
    } else if(sin_net->sa_family == AF_INET6) {
        for(i=0;i<4;i++) {
            r = rand();
            
            sin_addr_6->sin6_addr.s6_addr32[i] = 
                    (r & (~sin_smask_6->sin6_addr.s6_addr32[i])) |
                    (sin_net_6->sin6_addr.s6_addr32[i] & sin_smask_6->sin6_addr.s6_addr32[i]);

        }
    }
}


void set_mask(struct sockaddr * sin,int family, int mask) {
    uint32_t m[4] = {0,0,0,0};
    int i;
    struct sockaddr_in6 * sin_6 = sin;
    struct sockaddr_in * sin_4 = sin;
    
    srand(time(NULL));
    
    sin->sa_family = family;
    
    for(i=1;i<=mask;i++) {
        if(i>96) {
            m[3] = m[3] | (0x1 << (32 - i));
        }else if(i>64) {
            m[2] = m[2] | (0x1 << (32 - i));
        } else if(i>32) {
            m[1] = m[1] | (0x1 << (32 - i));
        } else {
            m[0] = m[0] | (0x1 << (32 - i));
        }
        
    }
    
    for (i=0;i<4;i++) {
        m[i] = htonl(m[i]);
    }
    if(family == AF_INET) {
        memcpy(&(sin_4->sin_addr.s_addr),&(m[0]),32);
    } else if(family == AF_INET6) {
        memcpy(&(sin_6->sin6_addr.s6_addr32[0]),&(m[0]),128);
    }
    
}


unsigned short int inet_cksum(unsigned short int *addr, size_t len) {
    register int nleft = (int) len;
    register unsigned short int *w = addr;
    unsigned short int answer = 0;
    register int sum = 0;

    /*
     *  Our algorithm is simple, using a 32 bit accumulator (sum),
     *  we add sequential 16 bit words to it, and at the end, fold
     *  back all the carry bits from the top 16 bits into the lower
     *  16 bits.
     */
    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    /* mop up an odd byte, if necessary */
    if (nleft == 1) {
        *(u_char *) (&answer) = *(u_char *) w;
        sum += answer;
    }

    /*
     * add back carry outs from top 16 bits to low 16 bits
     */
    sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
    sum += (sum >> 16); /* add carry */
    answer = ~sum; /* truncate to 16 bits */
    return (answer);
}

int getipaddr(char * name, struct sockaddr * sin,int family) {
    struct addrinfo * res;
    struct addrinfo * info;
    struct addrinfo hints = {0};
    int error;
    int found = 0;
    
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = family;
    
    error = getaddrinfo(name,NULL,&hints,&res);
    if (error != 0)
    {   
        if (error == EAI_SYSTEM)
        {
            perror("getaddrinfo");
        }
        else
        {
            fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
        }   
        return -1;
    }
    else {
        for(info = res;info != NULL && found == 0; info = info->ai_next) {
            switch(info->ai_addr->sa_family) {
                case AF_INET:
                case AF_INET6:
                    memcpy(sin,info->ai_addr,info->ai_addrlen);
                    found = 1;
                    break;
            }
        }
        
        freeaddrinfo(res);
    }
    if(found == 1){
        return 0;
    }
    else {
        return -2;
    }
}
