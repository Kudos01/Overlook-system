//_danny_file_processor_ Library

#ifndef _file_processor_H
#define _file_processor_H

//for the directory flag O_DIRECTORY
#define _POSIX_C_SOURCE 200809L

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

//#include "frame.h"

//Constants
typedef struct {
    char *  weather_station;
    char *  file_directory;
    int     time_to_wait;
    char *  ip_jack;
    int     port_jack;
    char *  ip_wendy;
    int     port_wendy;
}ConfigDanny;

typedef struct {
    char *  ip_jack;
    int     port_jack;
}ConfigJack;

typedef struct {
    //Perhaps we need the name of the file where we got the data from?
    char * date;
    char * hour;
    char * temperature;
    char * humidity;
    char * atmosphere_pressure;
    char * precipiataion;
}WeatherFormat;


//Functions and Procedures

//function to load the config from the given file
void loadDannyConfig(int fd, ConfigDanny * config);

void loadJackConfig(int fd, ConfigJack * config);

WeatherFormat processTextFile(int fd);

WeatherFormat * checkTxtFiles(ConfigDanny config, int * how_many_txt_files);

void printWeatherStationInfo(WeatherFormat data);

#endif
