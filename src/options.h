/* 
 * File:   options.h
 * Author: targuan
 *
 * Created on 29 mai 2013, 23:20
 */

#ifndef OPTIONS_H
#define	OPTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    struct options {
        /**
         * 
         * Output options
         */
        char *out_file_name;
        char *in_file_name;
        int count;

        /**
         * NET Options
         */
        char *smac; // source MAC
        char *dmac; // destination MAC
        char *dip; // destination IP
        char *snet; // source IP
        int smask; //source MASK
        int family;
        
        int caplen;
        
        char *interface;
        int mtu;
    };
    void setoptions(int argc, char** argv,struct options * opt);

#ifdef	__cplusplus
}
#endif

#endif	/* OPTIONS_H */

