#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>

#include "bmp.h"
#include "table.h"

int cliFd;

extern int init_tcp();

extern void *send_message(Usr_t *usr);

extern void start_backstage_secv();

void *recv_message(void *arg);


