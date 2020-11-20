#include "DannyConnections.h"

int connectToJack (ConfigDanny config){
  char aux_frame[FRAME_SIZE];
  char buffer_str[350];

  //char aux_source[SOURCE_SIZE];
  //char aux_data[DATA_SIZE];
  //char type = '2';

  //bzero(aux_source, SOURCE_SIZE);
  //bzero(aux_data, DATA_SIZE);

/*
  getFrameData(aux_source, &type, aux_data, aux_frame);
  sprintf(buffer_str, "source recieved: |%s|\n", aux_source);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "type recieved: |%c|\n", type);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "data recieved: |%s|\n", aux_data);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
  */


  uint16_t port;
  //ports are between 1 and 65535
  if (config.port_jack < 8859 || config.port_jack  > 8880){
      sprintf(buffer_str, "Error: For this project, only ports between 8860 and 8879 are allowed (inclusive)\n");
      write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
      exit (EXIT_FAILURE);
  }
  //assign value to port
  port = config.port_jack;

  // Check if the IP is valid and convert it to binary format
  struct in_addr ip_addr;
  if (inet_aton (config.ip_jack, &ip_addr) == 0)
  {
      fprintf (stderr, "inet_aton (%s): %s\n", config.ip_jack, strerror (errno));
      exit (EXIT_FAILURE);
  }

  // Create the socket
  int sockfd;
  sockfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
  //if we didn't get the socket fd
  if (sockfd < 0)
  {
    sprintf(buffer_str, "Error: couldn't get socket fd\n");
    write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
      exit (EXIT_FAILURE);
  }

  // Specify the adress and port of the remote host
  //client needs to know server and vice versa
  struct sockaddr_in s_addr;
  bzero (&s_addr, sizeof (&s_addr));
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons (port);
  s_addr.sin_addr = ip_addr;

  // We can connect to the server casting the struct:
  // bind waits for a struct sockaddr * and we are passing struct sockaddr_in *
  if (connect (sockfd, (void *) &s_addr, sizeof (s_addr)) < 0){
      perror ("connect");
      exit (EXIT_FAILURE);
  }

  fillFrame(SOURCE_DANNY, TYPE_CONNECT, config.weather_station, aux_frame);

  /*
  sprintf(buffer_str, "Source contents: |%s|\n", aux_frame);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "Frame size: |%ld|\n", strlen(aux_frame));
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "Source size: |%ld|\n", strlen(aux_source));
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  getFrameData(aux_source, &type, aux_data, aux_frame);

  sprintf(buffer_str, "Source size after: |%ld|\n", strlen(aux_source));
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "Source contents: |%s|\n", aux_source);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "Type contents: |%c|\n", type);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "Data contents: |%s|\n", aux_data);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
  */

  sendFrame(sockfd, aux_frame);

  read(sockfd, aux_frame, FRAME_SIZE);

  sprintf(buffer_str, "frame: %s\n", aux_frame);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
  /*
  if(i < 0){
    sprintf(buffer_str, "Error\n");
    write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  }else{
    sprintf(buffer_str, "got the ok aux_frame\n");
    write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
  }
  */
  return sockfd;

  //TODO: JACK NEEDS TO CHECK IF THE FRAME THEY RECIEVE IS OKAY AND RETURN ERROR FRAME

  //TODO: DANNY NEEDS TO CHECK IF THE FRAME THEY RECIEVE IS OKAY AND RETURN ERROR FRAME

}
