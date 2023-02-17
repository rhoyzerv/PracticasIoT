#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <cjson/cJSON.h>


#define PORT 8080
#define MAX_CLIENTS 5

int json(){
    // Open the JSON file
  FILE* file = fopen("data.json", "r");
  if (!file) {
      printf("Failed to open file\n");
      return 1;
  }

  // Read the contents of the file into a string
  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);
  char* data = (char*)malloc(length + 1);
  fread(data, 1, length, file);
  data[length] = '\0';

  // Parse the JSON string
  cJSON* root = cJSON_Parse(data);
  if (!root) {
      printf("Failed to parse JSON\n");
      return 1;
  }

  // Get the array from the root object
  cJSON* array = cJSON_GetObjectItem(root, "");

  // Iterate over the array and print the values of each object
  for (int i = 0; i < cJSON_GetArraySize(array); i++) {
      cJSON* object = cJSON_GetArrayItem(array, i);
      const char* name = cJSON_GetObjectItem(object, "name")->valuestring;
      int x = cJSON_GetObjectItem(object, "x")->valueint;
      int y = cJSON_GetObjectItem(object, "y")->valueint;
      int z = cJSON_GetObjectItem(object, "z")->valueint;
      int xyz = cJSON_GetObjectItem(object, "xyz")->valueint;
      printf("Name: %s, x: %d, y: %d, z: %d, xyz: %d\n", name, x, y, z, xyz);
  }

  // Free memory and close the file
  cJSON_Delete(root);
  free(data);
  fclose(file);

  return 0;
}

void *handle_client(void *arg) {
  int client_socket = *(int *)arg;

  // Receive data from the client
  char buffer[1024] = {0};
  int valread = read(client_socket , buffer, 1024);
  // printf("Received message from client: %s\n", buffer);

  // Send data to the client
  //char *hello = "Hello from server";
  send(client_socket , buffer , strlen(buffer) , 0);
  json();
  printf("Message sent to client\n");

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
    printf("ID: %d\n",threads[thread_count]);
  }

  return 0;
}
