
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include "ip.h"


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
