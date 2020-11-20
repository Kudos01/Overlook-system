#ifndef _JackWendyConnections_H
#define _JackWendyConnections_H

#define _GNU_SOURCE

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>

#include "file_processor.h"
#include "frame.h"

void startJack(ConfigJack config);

#endif
