#include "options.h"
#include <getopt.h>
#include <stdlib.h>  
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>

static struct option long_options[] = {
    {"smac", required_argument, 0, 'e'},
    {"dmac", required_argument, 0, 'r'},
    {"snet", required_argument, 0, 's'},
    {"dip", required_argument, 0, 'd'},
    {"smask", required_argument, 0, 'm'},
    {"queries", required_argument, 0, 'q'},
    {"output", required_argument, 0, 'o'},
    {"count", required_argument, 0, 'c'},
    {"family", required_argument, 0, 'f'},
    {"interface", required_argument, 0, 'i'},
    {"mtu", required_argument, 0, 't'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
};

void printoptions (struct options_args * opt) {
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

    char *interface;
    int mtu;
        
        
    if(opt->out_file_name != NULL)
        fprintf(stderr,"Writing result in %s\n",opt->out_file_name);
    else if(opt->interface != NULL)
        fprintf(stderr,"Sending result on %s\n",opt->interface);
    else
        fprintf(stderr,"No output specified\n");
    
    if(opt->in_file_name != NULL)
        fprintf(stderr,"Reading queries from %s\n",opt->in_file_name);
    else 
        fprintf(stderr,"No queries file specified\n");
    
    
}

void setoptions(int argc, char** argv, struct options_args * opt) {
    opt->out_file_name = NULL;
    opt->in_file_name = NULL;
    opt->count = 0;
    opt->smac = NULL; // source MAC
    opt->dmac = NULL; // destination MAC
    opt->dip = NULL; // destination IP
    opt->snet = NULL; // source IP
    opt->smask = 128; //source MASK
    opt->family = 0;
    opt->mtu = 1200;
    opt->interface = NULL;



    int c = 0;

    while (c != -1) {
        int option_index = 0;

        c = getopt_long(argc, argv, "e:r:s:d:m:q:o:c:hl:i:",
                long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 'e':
                opt->smac = malloc(strlen(optarg)+1);
                strcpy(opt->smac,optarg);
                break;
            case 'r':
                opt->dmac = malloc(strlen(optarg)+1);
                strcpy(opt->dmac,optarg);
                break;
            case 's':
                opt->snet = malloc(strlen(optarg)+1);
                strcpy(opt->snet,optarg);
                break;
            case 'd':
                opt->dip = malloc(strlen(optarg)+1);
                strcpy(opt->dip,optarg);
                break;
            case 'm':
                opt->smask = atoi(optarg);
                break;
            case 'q':
                opt->in_file_name = malloc(strlen(optarg)+1);
                strcpy(opt->in_file_name,optarg);
                break;
            case 'o':
                if(strcmp(optarg, "-") == 0) {
                    opt->out_file_name = "stdin";
                } else {
                    opt->out_file_name = malloc(strlen(optarg)+1);
                    strcpy(opt->out_file_name,optarg);
                }
                break;
            case 'c':
                opt->count = atoi(optarg);
                break;
            case 'f':
                if(strcmp(optarg,"4") == 0) {
                    opt->family = AF_INET;
                } else if(strcmp(optarg,"6") == 0) {
                    opt->family = AF_INET6;
                } else {
                    opt->family = 0;
                }
                break;
            case 'i':
                opt->interface = malloc(strlen(optarg)+1);
                strcpy(opt->interface,optarg);
                break;
            case 't':
                opt->mtu = atoi(optarg);
                break;
            case 'h':
                break;
        }

    }
    
    printoptions (opt);
}
