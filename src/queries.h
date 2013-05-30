/* 
 * File:   queries.h
 * Author: cqdc5422
 *
 * Created on 30 mai 2013, 10:18
 */

#ifndef QUERIES_H
#define	QUERIES_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdio.h>
void init_domain_file(FILE** fp_domain,char *filename);
int nextdomain(FILE* fp_domain,char **domain,int *qtype,int *qclass);


#ifdef	__cplusplus
}
#endif

#endif	/* QUERIES_H */

