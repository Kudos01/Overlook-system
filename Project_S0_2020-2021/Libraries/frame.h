#ifndef _frame_H
#define _frame_H

#define _GNU_SOURCE
#define SOURCE_SIZE 14
#define DATA_SIZE 100
#define FRAME_SIZE 115

#define SOURCE_DANNY "DANNY"
#define SOURCE_JACK "JACK"
#define SOURCE_WENDY "WENDY"
#define DATA_ERROR "ERROR"
#define DATA_CONNECT_OK "CONNECTION OK"
#define TYPE_CONNECT "C"
#define TYPE_DATA "D"
#define TYPE_DISCONNECT "Q"
#define TYPE_OK_CONNECT "O"
#define TYPE_OK_DATA "B"
#define TYPE_ERROR_CONNECT "E"
#define TYPE_ERROR_DATA "K"
#define TYPE_ERROR_FRAME "Z"

//Includes
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <dirent.h>
#include <errno.h>

#include "file_processor.h"

typedef struct {
    char    source[SOURCE_SIZE];
    char    type;
    char    data[DATA_SIZE];
}Frame;

void fillFrame(char * source, char * type, char * data, char * frame);

void getFrameData(Frame * frame, char * aux_frame);

void sendFrame(int sockfd, char * frame);

void readFrame(int sockfd, char * frame);

//void generateWeatherString(WeatherFormat data, char * formatted_data);


#endif
