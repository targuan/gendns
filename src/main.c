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
#include "queries.h"

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
    struct sockaddr *sin_net;
    struct sockaddr *sin_dst;
    struct sockaddr *sin_mask;
    int sendsize;
    struct options opts;
    FILE* fp_domain;
    char * domain;

    int q_class;
    int q_type;



    /**
     * 
     * headers
     */
    struct ether_header *eth;
    struct iphdr *ip;
    struct ip6_hdr *ip6;
    struct udphdr *udp;
    u_char *dns;

    setoptions(argc, argv, &opts);

    header.caplen = opts.caplen;
    header.ts.tv_sec = 0;
    header.ts.tv_usec = 0;

    // @TODO
    // check return values for errors
    p = pcap_open_dead(DLT_EN10MB, opts.caplen);
    dumper = pcap_dump_open(p, opts.out_file_name);

    init_domain_file(&fp_domain, opts.in_file_name);

    if (fp_domain == NULL) {
        exit(4);
    }




    packet = calloc(sizeof (u_char) * 4096, 1);
    sin_src = calloc(sizeof (struct sockaddr_storage), 1);
    sin_net = calloc(sizeof (struct sockaddr_storage), 1);
    sin_dst = calloc(sizeof (struct sockaddr_storage), 1);
    sin_mask = calloc(sizeof (struct sockaddr_storage), 1);
    domain = calloc(sizeof (char) * 1024, 1);


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
    if (getipaddr(opts.dip, sin_dst, opts.family) < 0) {
        fprintf(stderr, "dip error\n");
        return 1;
    }
    if (getipaddr(opts.sip, sin_net, opts.family) < 0) {
        fprintf(stderr, "sip error\n");
        return 1;
    }

    if (sin_net->sa_family != sin_dst->sa_family) {
        fprintf(stderr, "IP family error\n");
        return 2;
    }

    set_mask(sin_mask, sin_net->sa_family, opts.smask);

    if (sin_net->sa_family == AF_INET) {
        eth->ether_type = htons(ETHERTYPE_IP);

        ip = (struct iphdr *) (packet + ETHER_HDR_LEN);
        ip->saddr = ((struct sockaddr_in *) sin_net)->sin_addr.s_addr;
        ip->daddr = ((struct sockaddr_in *) sin_dst)->sin_addr.s_addr;


        ip->version = 4;
        ip->ihl = 5;
        ip->frag_off = 0;
        ip->id = 0;
        ip->protocol = IPPROTO_UDP;
        ip->tos = 0;
        ip->ttl = IPDEFTTL;
        //ip->tot_len

        udp = (struct udphdr *) (((void *) ip) + IP_HDR_LEN);
    } else if (sin_net->sa_family == AF_INET6) {
        eth->ether_type = htons(ETHERTYPE_IPV6);

        ip6 = (struct ip6_hdr *) (packet + ETHER_HDR_LEN);
        ip6->ip6_vfc = 6 << 4;
        ip6->ip6_hlim = IPDEFTTL;
        ip6->ip6_nxt = IPPROTO_UDP;

        memcpy(&(ip6->ip6_src), &(((struct sockaddr_in6 *) sin_net)->sin6_addr), 16);
        memcpy(&(ip6->ip6_dst), &(((struct sockaddr_in6 *) sin_dst)->sin6_addr), 16);

        udp = (struct udphdr *) (((void *) ip6) + IP6_HDR_LEN);
    } else {
        return 3;
    }
    /**
     * 
     * UDP
     */
    udp->check = 0;
    udp->dest = htons(53);


    dns = (u_char *) (((void *) udp) + UDP_HDR_LEN);






    for (i = 0; i < opts.count; i++) {
        udp->source = htons(rand());

        nextdomain(fp_domain, &domain, &q_type, &q_class);

        sendsize = res_mkquery(QUERY, domain, q_class, q_type, NULL,
                0, NULL, dns, PACKETSZ);

        udp->len = htons(sendsize + UDP_HDR_LEN);

        get_rand_addr(sin_net, sin_mask, sin_src);

        if (sin_net->sa_family == AF_INET) {
            ip->saddr = ((struct sockaddr_in *) sin_src)->sin_addr.s_addr;
            ip->tot_len = htons(sendsize + UDP_HDR_LEN + (ip->ihl * 4));
            header.len = sendsize + UDP_HDR_LEN + (ip->ihl * 4) + ETHER_HDR_LEN;

            ip->check = 0;
            ip->check = inet_cksum(ip, (ip->ihl * 4));
        } else if (sin_net->sa_family == AF_INET6) {
            memcpy(&(ip6->ip6_src), &(((struct sockaddr_in6 *) sin_src)->sin6_addr), 16);
            ip6->ip6_plen = htons(sendsize + UDP_HDR_LEN);
            header.len = sendsize + UDP_HDR_LEN + sizeof (struct ip6_hdr) +ETHER_HDR_LEN;
        }


        if (header.len > header.caplen) {
            fprintf(stderr, "too long: %s needs %d\n", domain,header.len);
        } else {
                pcap_dump(dumper, &header, packet);
        }
    }


    pcap_dump_close(dumper);
    pcap_close(p);
    fclose(fp_domain);

    return (EXIT_SUCCESS);
}
