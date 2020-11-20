#include "frame.h"

void fillFrame(char * source, char * type, char * data, char * frame){

  char aux_source[SOURCE_SIZE];
  char aux_data[DATA_SIZE];

  //bzero(aux_source, SOURCE_SIZE);
  //bzero(aux_data, DATA_SIZE);

  strcpy(aux_source, source);

  //fill part that isnt source data with \0
  for (int i = strlen(source); i < SOURCE_SIZE; i++) {
    aux_source[i] = '\0';
  }

  strcpy(aux_data, data);

  //fill part that isnt data with \0
  for (int i = strlen(data); i < DATA_SIZE; i++) {
    aux_data[i] = '\0';
  }

  //put \0 at the end so that the string ends
  aux_data[DATA_SIZE-1] = '\0';

  int index=0;
  for (int i = 0; i < FRAME_SIZE; i++) {

    if(i < SOURCE_SIZE){
      frame[i] = aux_source[index];
      index++;

    }else if(i == SOURCE_SIZE){
      frame[i] = *type;
      index=0;

    }else{
      frame[i] = aux_data[index];
      index++;
    }
  }
}

void getFrameData(Frame * frame, char * aux_frame){

  //char buffer_str[255];

  for (int i = 0; i < SOURCE_SIZE; i++) {
      frame->source[i] = aux_frame[i];
  }

  frame->type = aux_frame[SOURCE_SIZE];

  int index=0;
  for (int i = SOURCE_SIZE+1; i < FRAME_SIZE; i++) {
      frame->data[index] = aux_frame[i];
      index++;
  }

  //sprintf(buffer_str, "Source INSIDE: %s\n", source);
  //write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  //sprintf(buffer_str, "Type INSIDE: %c\n", *type);
  //write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  //sprintf(buffer_str, "Data INSIDE: %s\n", data);
  //write(STDOUT_FILENO, buffer_str, strlen(buffer_str));


}

void sendFrame(int sockfd, char * frame){
  //char buffer_str[100];

  write(sockfd, frame, FRAME_SIZE);

  //sprintf(buffer_str, "strlen size frame: %ld\n", strlen(frame));
  //write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
}

void readFrame(int sockfd, char * frame){
  char aux_frame[115];

  read(sockfd, aux_frame, FRAME_SIZE+1);

  strcpy(frame, aux_frame);
}
