/* 
 * File:   main.c
 * Author: cqdc5422
 *
 * Created on 29 mai 2013, 16:45
 */

#include <stdio.h>
#include <stdlib.h>
#include <resolv.h>
#include <pcap.h>
#include <string.h>
#include <sys/time.h>
#include <net/ethernet.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "etnernet.h"
#include "options.h"
#include "ip.h"

#define UDP_HLEN	8
#define UDP_HDR_LEN     UDP_HLEN

#define IP_HDR_LEN      (ip->ihl * 4)
#define IP6_HDR_LEN     (sizeof(struct ip6_hdr))


/*
 * 
 */
int main(int argc, char** argv) {
    pcap_dumper_t *dumper;
    pcap_t *p;
    struct pcap_pkthdr header;
    u_char *packet;
    int i;
    struct sockaddr *sin_src;
    struct sockaddr *sin_dst;
    int sendsize;
    struct options opts;
    
    

    /**
     * 
     * headers
     */
    struct ether_header *eth;
    struct iphdr *ip;
    struct ip6_hdr *ip6;
    struct udphdr *udp;
    u_char *dns;
    
    setoptions(argc, argv,&opts);

    header.caplen = opts.caplen;
    header.ts.tv_sec = 0;
    header.ts.tv_usec = 0;
    
    p = pcap_open_dead(DLT_EN10MB, opts.caplen);
    dumper = pcap_dump_open(p, opts.out_file_name);
    



    packet = calloc(sizeof (u_char) * 4096,1);
    sin_src = calloc(sizeof (struct sockaddr_storage),1);
    sin_dst = calloc(sizeof (struct sockaddr_storage),1);
            
            
    /**
     * 
     * ETHERNET
     */

    eth = (struct ether_header *) packet;


    if (ether_setaddr(opts.smac, &(eth->ether_shost)) < 0) {
        fprintf(stderr, "smac error\n");
        return 1;
    }
    if (ether_setaddr(opts.dmac, &(eth->ether_dhost)) < 0) {
        fprintf(stderr, "dmac error\n");
        return 1;
    }



    /**
     * 
     * IP
     */
    if(getipaddr(opts.dip, sin_dst,opts.family) <0 ) {
        fprintf(stderr, "dip error\n");
        return 1;
    }
    if(getipaddr(opts.sip, sin_src,opts.family) < 0){
        fprintf(stderr, "sip error\n");
        return 1;
    }

    if (sin_src->sa_family != sin_dst->sa_family) {
        fprintf(stderr, "IP family error\n");
        return 2;
    }

    if (sin_src->sa_family == AF_INET) {
        eth->ether_type = htons(ETHERTYPE_IP);
        
        ip = (struct iphdr *) (packet + ETHER_HDR_LEN);
        ip->saddr = ((struct sockaddr_in *) sin_src)->sin_addr.s_addr;
        ip->daddr = ((struct sockaddr_in *) sin_dst)->sin_addr.s_addr;

        ip->version = 4;
        ip->ihl = 5;
        ip->frag_off = 0;
        ip->id = 0;
        ip->protocol = IPPROTO_UDP;
        ip->tos = 0;
        ip->ttl = IPDEFTTL;
        //ip->tot_len

        udp = (struct udphdr *) (((void *) ip)+IP_HDR_LEN);
    }
    else if(sin_src->sa_family == AF_INET6) {
        eth->ether_type = htons(ETHERTYPE_IPV6);
        
        ip6 = (struct ip6_hdr *) (packet + ETHER_HDR_LEN);
        ip6->ip6_vfc = 6 << 4;
        ip6->ip6_hlim = IPDEFTTL;
        ip6->ip6_nxt = IPPROTO_UDP;
        
        memcpy(&(ip6->ip6_src),&(((struct sockaddr_in6 *)sin_src)->sin6_addr),16);
        memcpy(&(ip6->ip6_dst),&(((struct sockaddr_in6 *)sin_dst)->sin6_addr),16);
        
        udp = (struct udphdr *) (((void *) ip6)+IP6_HDR_LEN);
    } else {
        return 3;
    }  
    /**
     * 
     * UDP
     */
    udp->check = 0;
    udp->dest = htons(53);
    udp->source = htons(10000);
    //udp->len

    dns = (u_char *) (((void *) udp) + UDP_HDR_LEN);
    sendsize = res_mkquery(QUERY, "isc.org", C_IN, T_A, NULL,
            0, NULL, dns, PACKETSZ);
    
    udp->len = htons(sendsize + UDP_HDR_LEN);
    if (sin_src->sa_family == AF_INET) {
        ip->tot_len = htons(sendsize + UDP_HDR_LEN + (ip->ihl * 4));
        header.len = sendsize + UDP_HDR_LEN + (ip->ihl * 4) + ETHER_HDR_LEN;
    }
    else if(sin_src->sa_family == AF_INET6) {
        ip6->ip6_plen = htons(sendsize + UDP_HDR_LEN);
        header.len = sendsize + UDP_HDR_LEN + sizeof(struct ip6_hdr) + ETHER_HDR_LEN;
    }

    
    
    
    
    for (i = 0; i < opts.count; i++) {

        pcap_dump(dumper, &header, packet);
    }

    return (EXIT_SUCCESS);
}