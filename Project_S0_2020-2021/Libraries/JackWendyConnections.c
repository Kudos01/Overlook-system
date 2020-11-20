#include "JackWendyConnections.h"


void startJack (ConfigJack config){
  char buffer_str[200];
  char aux_frame[115];
  Frame frame;

  // Check if the port is valid
  uint16_t port;
  //ports are between 1 and 65535
  if (config.port_jack < 8859 || config.port_jack  > 8880){
      sprintf(buffer_str, "Error: For this project, only ports between 8860 and 8879 are allowed (inclusive)\n");
      write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
      exit (EXIT_FAILURE);
  }
  //assign value to port
  port = config.port_jack;

  // Create the socket
  // AF_INET is for IPv4, SOCK_STREAM is for using a connection oriented protocol and IPPROTO_TCP self explanatory
  //socket function: return offline fd
  //family, type, protocol
  int sockfd;
  sockfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
  //if we didn't get the socket fd
  if (sockfd < 0)
  {
    sprintf(buffer_str, "Error: couldn't get socket fd\n");
    write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
      exit (EXIT_FAILURE);
  }

  // Specify the address and port of the socket
  // We'll admit connexions to any IP of our machine and with the specified port

  //struct that configures socket parameters
  struct sockaddr_in s_addr;

  /*

  GUIDE FOR STRUCT

  struct sockaddr_in {
    uint8_t sin_len;		// Object size (automatically filled)
    sa_family_t sin_family;	// Socket family
    in_port_t sin_port;	// Port number (Big Endian)
    struct in_addr sin_addr;	// IP address (network byte ordered)
    char sin_zero[8];		// Padding filled with ’\0’
  };

  */

  bzero(&s_addr, sizeof(s_addr));
  s_addr.sin_family = AF_INET; // Socket family
  s_addr.sin_port = htons(port); // Port number (Big Endian)
  s_addr.sin_addr.s_addr = inet_addr(config.ip_jack); // IP address (network byte ordered)

  /*      BIND      */

  // When executing bind, we should add a cast:
  // bind waits for a struct sockaddr * and we are passing a struct sockaddr_in *
  //cast to void pointer!
  //if we have a socket without info
  if (bind (sockfd, (void *) &s_addr, sizeof (s_addr)) < 0){
    //print error
    sprintf(buffer_str, "Bind Error\n");
    write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
    exit (EXIT_FAILURE);
  }
  //otherwise open port
  // We now open the port (5 backlog queue, typical value)

  sprintf(buffer_str, "\nListening...\n"); //spritnf -- write formatted data to string
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  listen (sockfd, 5);

  while(1){
    //create new struct to fill with new client information that will be filled by accept function
    struct sockaddr_in c_addr;
    socklen_t c_len = sizeof (c_addr);

    // When executing accept we should add the same cast used in the bind function
    int newsock = accept (sockfd, (void *) &c_addr, &c_len);
    if (newsock < 0)
    {
        sprintf(buffer_str, "Accept error\n");
        write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
        exit (EXIT_FAILURE);
    }
    sprintf(buffer_str, "New connection from %s:%hu\n",
    inet_ntoa (c_addr.sin_addr), ntohs (c_addr.sin_port));
    write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

    int i = read(newsock, aux_frame, FRAME_SIZE);

    if(i < 0){
      sprintf(buffer_str, "Error\n");
      write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

      //send back an error frame
      fillFrame(SOURCE_JACK, TYPE_ERROR_CONNECT, DATA_ERROR, aux_frame);

      sendFrame(newsock, aux_frame);

    }
    else{
      sprintf(buffer_str, "frame: %s\n", aux_frame);
      write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

      getFrameData(&frame, aux_frame);

      fillFrame(SOURCE_JACK, TYPE_OK_CONNECT, DATA_CONNECT_OK, aux_frame);

      sendFrame(newsock, aux_frame);

      while (frame.type != 'Q') {
        read(newsock, aux_frame, FRAME_SIZE);

        //extract data
        getFrameData(&frame, aux_frame);

        //print the data received
        sprintf(buffer_str, "frame: %s\n", frame.data);
        write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
      }

    }
    close(sockfd);

  }
}
