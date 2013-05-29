

#include <net/ethernet.h>
#include <netinet/ether.h>
#include <string.h>

int ether_setaddr(char *mac, u_int8_t *destination) {
    struct ether_addr *temp_eth_addr;
    
    temp_eth_addr = ether_aton(mac);
    
    if(temp_eth_addr == 0) {
        return -1;
    }
    
    memcpy(destination,temp_eth_addr,ETH_ALEN);
    
    return 0;
}
