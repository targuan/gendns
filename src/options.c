#include "options.h"
#include <getopt.h>
#include <stdlib.h>  
#include <string.h>
#include <sys/socket.h>

void setoptions(int argc, char** argv, struct options * opt) {
    static struct option long_options[] = {
        {"smac", required_argument, 0, 'e'},
        {"dmac", required_argument, 0, 'r'},
        {"sip", required_argument, 0, 's'},
        {"dip", required_argument, 0, 'd'},
        {"smask", required_argument, 0, 'm'},
        {"queries", required_argument, 0, 'q'},
        {"output", required_argument, 0, 'o'},
        {"count", required_argument, 0, 'c'},
        {"caplen", required_argument, 0, 'l'},
        {"family", required_argument, 0, 'f'},
        {"interface", required_argument, 0, 'i'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    opt->out_file_name = "out.pcap";
    opt->in_file_name = "queries.log";
    opt->count = 2;
    opt->smac = "f8:d1:11:07:a4:74"; // source MAC
    opt->dmac = "00:07:cb:46:fa:98"; // destination MAC
    //char *dip = "80.12.195.184"; // destination IP
    opt->dip = "::1"; // destination IP
    //char *sip = "1.0.0.0"; // source IP
    opt->sip = "::2"; // source IP
    opt->smask = 8; //source MASK
    opt->family = 0;
    opt->caplen = 4096;



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
                opt->sip = malloc(strlen(optarg)+1);
                strcpy(opt->sip,optarg);
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
            case 'l':
                opt->caplen = atoi(optarg);
                break;
            case 'l':
                opt->interface = malloc(strlen(optarg)+1);
                strcpy(opt->interface,optarg);
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
            case 'h':
                break;
        }

        /*if (optind < argc) {
            printf("non-option ARGV-elements: ");
            while (optind < argc)
                printf("%s ", argv[optind++]);
            printf("\n");
        }*/


    }
}
