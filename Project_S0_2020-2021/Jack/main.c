//
// Created by Felipe Perez (felipe.perez)
// and Arcadia Youlten (arcadia.youlten)
// on 17/10/2020.
//
//ports
#define _GNU_SOURCE

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>  // For wait function
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <time.h>

//own libraries
#include "/users/home/felipe.perez/OS/Project_S0_2020-2021/Libraries/file_processor.h"
#include "/users/home/felipe.perez/OS/Project_S0_2020-2021/Libraries/JackWendyConnections.h"

//declares

#define WAITING "Waiting...\n"
#define NEW_CONNECTION "New Connection: \n"
#define GET_DATA " files found\n"
#define START_JACK "Starting Jack...\n"
#define DISCONNECT_JACK "\nDisconnecting Jack...\n\n"

//Global variables

char string[100];
ConfigJack config;
int fd_file_config;

void ksighandler(int signum){
    switch (signum){

        case SIGINT:
            sprintf(string, DISCONNECT_JACK); //spritnf -- write formatted data to string
            write(1, string, strlen(string));
            //potential to disconect any active connections in the future after this signal is recieved

            //FREES! AND CLOSE THE FILE!
            free(config.ip_jack);

            raise(SIGTERM);

            break;
/*
        case SIGALRM:
            //READ FILES IN THE INDICATED DIRECTORY
            sprintf(string, WAITING); //spritnf -- write formatted data to string
            write(STDOUT_FILENO, string, strlen(string));

            //checkTxtFiles(config);

            //reprogram the alarm
            //alarm(config.time_to_wait);
            break;
*/
        default:
          sprintf(string, "\n\nOther signal raised:%d\n\n", signum); //spritnf -- write formatted data to string
          write(STDOUT_FILENO, string, strlen(string));
            break;
    }
    // reprogram the executed signal to perform the same
    signal(signum, ksighandler);
}

int main(int argc, char *argv[]){
    //local variables

    //check if the user actually input a parameter
    if(argc > 1){
      //reading the config files
      fd_file_config = open(argv[argc-1], O_RDONLY, 0600);

      if(fd_file_config == -1){
          sprintf(string, "Unable to open config file!\n");
          write(STDOUT_FILENO, string, strlen(string));
      }else {

        //save config
        loadJackConfig(fd_file_config, &config);

        close(fd_file_config);

        signal(SIGINT, ksighandler);
        //signal(SIGALRM, ksighandler);

        sprintf(string, "Jack's IP: %s\n", config.ip_jack);
        write(STDOUT_FILENO, string, strlen(string));

        sprintf(string, "Jack's port: %d\n", config.port_jack);
        write(STDOUT_FILENO, string, strlen(string));

        startJack(config);

        while (1) {
          sleep(1);
        }


        //do the connection in another function? Cus when we get a CTRL+C we can handle it from here
        //and send a disconect msg to dannys
      }
    }else{
      sprintf(string, "Please input a file as a parameter\n");
      write(STDOUT_FILENO, string, strlen(string));
    }


    return(0);
}
