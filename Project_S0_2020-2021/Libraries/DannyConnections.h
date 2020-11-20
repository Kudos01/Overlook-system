#ifndef _DannyConnections_H
#define _DannyConnections_H

#define _GNU_SOURCE

#include "file_processor.h"
#include "frame.h"

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

int connectToJack(ConfigDanny config);

#endif
