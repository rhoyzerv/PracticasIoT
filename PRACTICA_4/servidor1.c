#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <json-c/json.h>


#define PORT 8080
#define MAX_CLIENTS 5

void *handle_client(void *arg) {
    FILE *fp;
    int client_socket = *(int *)arg;
    char buffernet[1024] = {0};
    char buffernets[1024] = {0};
    char buffer[1024];
    char numb[3];
    struct json_object *parsed_json;
    struct json_object *Accelerometer;
    struct json_object *Magnetometer;
    struct json_object *Gyroscope;
    struct json_object *AllSensors;

    struct json_object *value;

    size_t n_values;
    size_t i;

    fp = fopen("data.json","r");
    fread(buffer, 1024, 1, fp);
    fclose(fp);

    parsed_json = json_tokener_parse(buffer);

    json_object_object_get_ex(parsed_json, "Accelerometer", &Accelerometer);
    json_object_object_get_ex(parsed_json, "Magnetometer", &Magnetometer);
    json_object_object_get_ex(parsed_json, "Gyroscope", &Gyroscope);
    json_object_object_get_ex(parsed_json, "AllSensors", &AllSensors);

    int valread = read(client_socket , buffernet, 1024);
    int nexnet;
    strcpy(buffernets, "AA-");
    if(strcmp(buffernet,"1") == 0){
      strcat(buffernets, "1-");
      sleep(2);
      nexnet = read(client_socket , buffernet, 1024);
      i = nexnet - 1;
      sprintf(numb, "%d", i);
      strcat(buffernets, numb);
      value = json_object_array_get_idx(Accelerometer, i);
      strcat(buffernets , json_object_get_string(value));
      sprintf(numb, "%d", i+1);
      strcat(buffernets, numb);
    }
    if(strcmp(buffernet,"2") == 0){
      strcat(buffernets, "2-");
      sleep(2);
      nexnet = read(client_socket , buffernet, 1024);
      i = nexnet - 1;
      sprintf(numb, "%d", i);
      strcat(buffernets, numb);
      value = json_object_array_get_idx(Magnetometer, i);
      strcat(buffernets , json_object_get_string(value));
      sprintf(numb, "%d", i+2);
      strcat(buffernets, numb);

    }
    if(strcmp(buffernet,"3") == 0){
      strcat(buffernets, "3-");
      sleep(2);
      nexnet = read(client_socket , buffernet, 1024);
      i = nexnet - 1;
      sprintf(numb, "%d", i);
      strcat(buffernets, numb);
      value = json_object_array_get_idx(Gyroscope, i);
      strcat(buffernets , json_object_get_string(value));
      sprintf(numb, "%d", i+3);
      strcat(buffernets, numb);
    }
    if(strcmp(buffernet,"4") == 0){
      strcat(buffernets, "4-");
      sleep(2);
      nexnet = read(client_socket , buffernet, 1024);
      i = nexnet - 1;
      sprintf(numb, "%d", i);
      strcat(buffernets, numb);
      value = json_object_array_get_idx(AllSensors, i);
      strcat(buffernets , json_object_get_string(value));
      sprintf(numb, "%d", i+4);
      strcat(buffernets, numb);
    } 
    send(client_socket , buffernets , strlen(buffernets) , 0);
    printf("%s",buffernets);

    // Close the socket
    close(client_socket);

    return NULL;
}


int main(int argc, char const *argv[]) {
  int server_socket, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  // Create a socket
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Attach socket to the port 8080
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Listen for incoming connections
  if (listen(server_socket, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  printf("Server is listening on port %d\n", PORT);

  // Create a thread for each incoming connection
  pthread_t threads[MAX_CLIENTS];
  int thread_count = 0;
  while (1) {
    if ((new_socket = accept(server_socket, (struct sockaddr * )&address, &addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    printf("Accepted connection from client\n");

    if (pthread_create(&threads[thread_count++], NULL, handle_client, &new_socket) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}