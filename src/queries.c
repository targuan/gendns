
#include "queries.h"
#include <string.h>
#include <stdio.h>



void init_domain_file(FILE** fp_domain,char *filename) {
    *fp_domain = fopen(filename,"r");
    
    if(fp_domain == NULL) {
        perror("fopen() domain file");
    }
}
int nextdomain(FILE* fp_domain,char **domain,int *qtype,int *qclass) {
    char line[1024];
    char *type;
    char *class;
    
    *qtype = 1;
    *qclass = 1;
    
    if(feof(fp_domain)) {
        rewind(fp_domain);
    }
    fgets(line,1024,fp_domain);
    
    *domain=strtok(line,"\n\r \t");
    type = strtok(NULL,"\n\r \t");
    
    if(type != NULL) {
        if(strcasecmp(type,"A") == 0) {
            *qtype = 1;
        } else if(strcasecmp(type,"NS") == 0) {
            *qtype = 2;
        } else if(strcasecmp(type,"SOA") == 0) {
            *qtype = 6;
        } else if(strcasecmp(type,"PTR") == 0) {
            *qtype = 12;
        } else if(strcasecmp(type,"MX") == 0) {
            *qtype = 15;
        } else if(strcasecmp(type,"TXT") == 0) {
            *qtype = 16;
        } else if(strcasecmp(type,"SIG") == 0) {
            *qtype = 24;
        } else if(strcasecmp(type,"KEY") == 0) {
            *qtype = 25;
        } else if(strcasecmp(type,"AAAA") == 0) {
            *qtype = 28;
        } else if(strcasecmp(type,"SRV") == 0) {
            *qtype = 33;
        } else if(strcasecmp(type,"NAPTR") == 0) {
            *qtype = 35;
        } else if(strcasecmp(type,"RRSIG") == 0) {
            *qtype = 46;
        } else if(strcasecmp(type,"NSEC") == 0) {
            *qtype = 47;
        } else if(strcasecmp(type,"DNSKEY") == 0) {
            *qtype = 48;
        } else if(strcasecmp(type,"ANY") == 0) {
            *qtype = 255;
        } else {
            *qtype = 1;
        }
        
        class = strtok(NULL,"\n\r \t");
        if(class != NULL) {
            if(strcasecmp(type,"IN") == 0) {
                *qclass = 1;
            } else if(strcasecmp(type,"CH") == 0) {
                *qclass = 3;
            } else {
                *qclass = 1;
            }
                
            
        }
    }
    
    return 0;
}
