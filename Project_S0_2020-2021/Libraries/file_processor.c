#include "file_processor.h"

/*
  Function to read any type of string from a file with a specified separator
  by using the file descriptor
*/

void readStringFromFile(int fd, char ** str, char separator){

  char buffer[100];
  char buffer_str[200];
  int index=0;

  read(fd, &buffer[index], sizeof(char));

  //remove the / at the beggining of the word for the directory
  if(buffer[index] == '/'){
    index =0;
    read(fd, &buffer[index], sizeof(char));
  }


  while (buffer[index] != separator) {
      index++;
      read(fd, &buffer[index], sizeof(char));
  }

  buffer[index-1]= '\0';

  *str = (char *) malloc((index * sizeof(char))); //malloc length of string

  //copy the name to the final variable
  strcpy(*str, buffer);

  sprintf(buffer_str, "String INSIDE: %s\n", *str);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
}

/*
  Function to read any integer from a file with a specified separator
  by using the file descriptor
*/

void readNumberFromFile(int fd, int * num, char separator){

  char buffer[100];
  //char buffer_str[200];
  int index=0;

  //read the amount of time to wait between files
  index=0;
  read(fd, &buffer[index], sizeof(char));
  while (buffer[index] != separator) {
      index++;
      read(fd, &buffer[index], sizeof(char));
  }
  buffer[index]= '\0';

  *num = atoi(buffer);

  //sprintf(buffer_str, "Num: %d\n", *num);
  //write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
}

/*
  Get the information for the config file for Danny and put it in a struct that holds the given information
  the config variable is passed by reference.
  Heavliy uses readNumberFromFile and readStringFromFile
*/

void loadDannyConfig(int fd, ConfigDanny * config){

  //read the name of the weaher station
  readStringFromFile(fd, &(config->weather_station), '\n');

  //read the directory
  readStringFromFile(fd, &(config->file_directory), '\n');

  //Read the amount of time to wait between file checks.
  readNumberFromFile(fd, &(config->time_to_wait), '\n');

  //Read Jack's IP
  readStringFromFile(fd, &(config->ip_jack), '\n');

  //Read Jack's port
  readNumberFromFile(fd, &(config->port_jack), '\n');

  //Read Wendy's IP
  readStringFromFile(fd, &(config->ip_wendy), '\n');

  //Read Wendy's port
  readNumberFromFile(fd, &(config->port_wendy), '\n');

}

void loadJackConfig(int fd, ConfigJack * config){

  //Read Jack's IP
  readStringFromFile(fd, &(config->ip_jack), '\n');

  //Read Jack's port
  readNumberFromFile(fd, &(config->port_jack), '\n');
}

/*
  Get the information for the text files in the danny directory put it in a struct that holds the given information
  heavliy uses  readStringFromFile
*/
WeatherFormat processTextFile(int fd){
  WeatherFormat data;
  //read date
  readStringFromFile(fd, &(data.date), '\n');
  //read hour
  readStringFromFile(fd, &(data.hour), '\n');
  //read temp
  readStringFromFile(fd, &(data.temperature), '\n');
  //read humidity
  readStringFromFile(fd, &(data.humidity), '\n');
  //read pressure
  readStringFromFile(fd, &(data.atmosphere_pressure), '\n');
  //read percipiataion
  readStringFromFile(fd, &(data.precipiataion), '\n');

  return data;
}

void printWeatherStationInfo(WeatherFormat data){

  char buffer_str[500];

  sprintf(buffer_str, "\n%s\n", data.date);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "%s\n", data.hour);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "%s\n", data.temperature);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "%s\n", data.humidity);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "%s\n", data.atmosphere_pressure);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

  sprintf(buffer_str, "%s\n", data.precipiataion);
  write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

}

/*
Determine if a file is a .txt or not. we do this by passing the function the string of the file d_name
we read until the ., and then check if the information after it is txt or not
*/
int isTxt(char * filename){
  char buffer_str[10];
  int flag=0;
  int j=0;

  for (int i = 0; i < (int) strlen(filename); i++) {
    if(flag ==1){
      buffer_str[j]= filename[i];
      j++;
    }
    if(filename[i] == '.'){
      flag =1;
    }
  }
  buffer_str[j] = '\0';

  return !strcmp("txt", buffer_str);
}
/*
Delete a file from the directory that we want using the remove() function from stdio.h
*/
int deleteFile(char * full_file_path){
  int r=0;
  return r = remove(full_file_path);
}
/*

*/
WeatherFormat * checkTxtFiles(ConfigDanny config, int * how_many_txt_files){
    //int fd_txt_file;
    struct dirent *dp;
    int fd_directory;
    int fd_file;
    DIR *d;
    size_t i=0;
    size_t j=0;
    int num_files=0;
    int num_txt_files=0;
    int counter=0;

    //WeatherFormat data;
    WeatherFormat * data = NULL;

    char buffer_str[300];
    char full_file_path[200];

    if ((d = fdopendir((fd_directory = open(config.file_directory, O_RDONLY)))) == NULL) {
      sprintf(buffer_str, "Unable to open directory!\n");
      write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
        exit(1);
    }else{
      //count the number of files and print them
      while ((dp = readdir(d)) != NULL) {
        if (dp->d_name[0] == '.')
        continue;

        //counter for the amount of txt files
        if(isTxt(dp->d_name)){
          num_txt_files++;
        }

        num_files++;
      }

      sprintf(buffer_str, "Found %d File(s)\n", num_files);
      write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

      //malloc the space for the number of txt files found (if we found any)
      if(num_txt_files)
      data = (WeatherFormat *) malloc(sizeof(WeatherFormat) * num_txt_files);

      *how_many_txt_files = num_txt_files;

      //reset the directory pointer to the beggining
      rewinddir(d);

      while ((dp = readdir(d)) != NULL) {
        //skip hidden files
        if (dp->d_name[0] == '.')
        continue;
        //print the filename if the file is a txt
          sprintf(buffer_str, "File name: %s\n", dp->d_name);
          write(STDOUT_FILENO, buffer_str, strlen(buffer_str));

          if (isTxt(dp->d_name)) {

            //get the full file path to open it
            for (i = 0; i < strlen(config.file_directory); i++) {
                full_file_path[i] = config.file_directory[i];
            }

            full_file_path[i] = '/';
            i++;
            for (j = 0; j < strlen(dp->d_name); j++) {
                full_file_path[i] = dp->d_name[j];
                i++;
            }
            full_file_path[i] = '\0';

            //open the file we found to be a txt
            fd_file = open(full_file_path, O_RDONLY, 0600);
            if(fd_file == -1){

                sprintf(buffer_str, "Unable to open weather file!\n");
                write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
            }

            else{
              //process the file
              data[counter] = processTextFile(fd_file);

              counter++;

            }

            //close the file
            close(fd_file);
            //delete the file

            if(deleteFile(full_file_path) == 1){
              sprintf(buffer_str, "Problem deleting file: %s\n", dp->d_name);
              write(STDOUT_FILENO, buffer_str, strlen(buffer_str));
            }
        }

    }

      closedir(d); // note this implicitly closes dfd
    }

    //return array
    return data;
    //free malloc'd memory OUTSIDE!!!
}
