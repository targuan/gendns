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
        //char *dip = "80.12.195.184"; // destination IP
        char *dip; // destination IP
        //char *sip = "1.0.0.0"; // source IP
        char *sip; // source IP
        int smask; //source MASK
        int family;
        
        int caplen;
        
        char *interface;
    };
    void setoptions(int argc, char** argv,struct options * opt);

#ifdef	__cplusplus
}
#endif

#endif	/* OPTIONS_H */

