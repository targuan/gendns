#include "options.h"
#include <getopt.h>
#include <stdlib.h>  
#include <string.h>
#include <sys/socket.h>

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

void setoptions(int argc, char** argv, struct options * opt) {
    opt->out_file_name = "out.pcap";
    opt->in_file_name = "queries.log";
    opt->count = 0;
    opt->smac = ""; // source MAC
    opt->dmac = ""; // destination MAC
    opt->dip = ""; // destination IP
    opt->snet = ""; // source IP
    opt->smask = 128; //source MASK
    opt->family = 0;
    opt->mtu = 1200;



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
                opt->out_file_name = malloc(strlen(optarg)+1);
                strcpy(opt->out_file_name,optarg);
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
                opt->caplen = atoi(optarg);
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
}
