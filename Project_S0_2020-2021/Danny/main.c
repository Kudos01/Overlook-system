//
// Created by Felipe Perez (felipe.perez)
// and Arcadia Youlten (arcadia.youlten)
// on 17/10/2020.
//
#include <sys/types.h>
#include <sys/wait.h>  // For wait function
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

//own libraries
#include "../Libraries/file_processor.h"
#include "../Libraries/DannyConnections.h"
#include "../Libraries/frame.h"

//declares

#define TESTING "Testing...\n"
#define NO_FILES_AVALIABLE "No files avaliable\n"
#define FILES_FOUND " files found\n"
#define START_DANNY "Starting Danny...\n"
#define DISCONNECT_DANNY "\nDisconnecting Danny...\n\n"

//Global variables

char string[100];
ConfigDanny config;
int fd_file_config;
int sockfd;
WeatherFormat * data = NULL;
//Frame frame;
char aux_frame[FRAME_SIZE];
int num_txt_files = 0;

void generateWeatherString(WeatherFormat data, char * formatted_data){
  bzero(formatted_data, strlen(formatted_data));
  strcpy(formatted_data, data.date);
  strcat(formatted_data, "#");
  strcat(formatted_data, data.hour);
  strcat(formatted_data, "#");
  strcat(formatted_data, data.temperature);
  strcat(formatted_data, "#");
  strcat(formatted_data, data.humidity);
  strcat(formatted_data, "#");
  strcat(formatted_data, data.atmosphere_pressure);
  strcat(formatted_data, "#");
  strcat(formatted_data, data.precipiataion);
}

void ksighandler(int signum){
    switch (signum){

        case SIGINT:
            sprintf(string, DISCONNECT_DANNY); //spritnf -- write formatted data to string
            write(1, string, strlen(string));
            //potential to disconect any active connections in the future after this signal is recieved

            //Send disconnection FRAME

            fillFrame(SOURCE_DANNY, TYPE_DISCONNECT, config.weather_station, aux_frame);
            sendFrame(sockfd, aux_frame);

            //FREES! AND CLOSE THE FILE!
            free(config.weather_station);
            free(config.file_directory);
            free(config.ip_jack);
            free(config.ip_wendy);


            close(sockfd);

            raise(SIGINT);

            break;

        case SIGALRM:
            //READ FILES IN THE INDICATED DIRECTORY
            sprintf(string, TESTING); //spritnf -- write formatted data to string
            write(STDOUT_FILENO, string, strlen(string));

            data = checkTxtFiles(config, &num_txt_files);

            if(data != NULL){
              char formatted_data[100];
              //char buffer_str[250];

              for (int i = 0; i < num_txt_files; i++) {
                generateWeatherString(data[i], formatted_data);
                /*
                sprintf(buffer_str, "data length: %ld\n", strlen(formatted_data));
                write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

                sprintf(buffer_str, "data: %s\n", formatted_data);
                write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
                */
                fillFrame(SOURCE_DANNY, TYPE_DATA, formatted_data, aux_frame);
                sendFrame(sockfd, aux_frame);

              }
              //send weather station info

            }


            free(data);
            data = NULL;

            //reprogram the alarm
            alarm(config.time_to_wait);
            break;

        default:
          sprintf(string, "\n\nOther signal raised:%d\n\n", signum); //spritnf -- write formatted data to string
          write(STDOUT_FILENO, string, strlen(string));
            break;
    }
    /* reprogram the executed signal to perform the same
     */
    signal(signum, ksighandler);
}

int main(int argc, char *argv[]){
    //local variables
    //int sockfd;
    //check if the user actually input a parameter
    if(argc > 1){
      //reading the config files
      fd_file_config = open(argv[argc-1], O_RDONLY, 0600);

      if(fd_file_config == -1){
          sprintf(string, "Unable to open config file!\n");
          write(STDOUT_FILENO, string, strlen(string));
      }else {
        //save config
        loadDannyConfig(fd_file_config, &config);

        close(fd_file_config);

        sprintf(string, "Weather station: %s\n", config.weather_station);
        write(STDOUT_FILENO, string, strlen(string));

        sprintf(string, "Weather station: %s\n", config.file_directory);
        write(STDOUT_FILENO, string, strlen(string));

        sprintf(string, "Weather station: %d\n", config.time_to_wait);
        write(STDOUT_FILENO, string, strlen(string));

        sprintf(string, "Weather station: %s\n", config.ip_jack);
        write(STDOUT_FILENO, string, strlen(string));

        sprintf(string, "Weather station: %d\n", config.port_jack);
        write(STDOUT_FILENO, string, strlen(string));

        sprintf(string, "|TEST|\n");
        write(STDOUT_FILENO, string, strlen(string));

        signal(SIGINT, ksighandler);
        signal(SIGALRM, ksighandler);

        //TODO: SEND A SPECIFIC FRAME REQUESTING CONNECTION

        //make it return the fd here and use it to send everytime we get a signal (what we programmed in phase 1)
        //if connect to jack fails, we exit and print a msg (though we already do it inside)
        sockfd = connectToJack(config);

        //once we have the fd here, we can fillframe, sendFrame, and wait for the ANSWER
        //if answer good, then we do nothing and keep going.
        //if answer bad, we check the report on what to do

        //TODO: GET ANSWER FROM JACK/WENDY
        //TODO: DISCONNECT PROCEDURE

        alarm(config.time_to_wait);

        while (1) {
          pause();
        }

      }
    }else{
      sprintf(string, "Please input a file as a parameter\n");
      write(STDOUT_FILENO, string, strlen(string));
    }


    return(0);
}
