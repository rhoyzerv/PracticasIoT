#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define SERVER_ADDRESS "10.0.0.102"

void *send_request(void *arg) {
  int client_socket = *(int *)arg;

  // Send data to the server
  int n1,n2;
  printf("\n N1: ");
  scanf("%d",&n1);
  printf("\n N2: ");
  scanf("%d",&n2);
  char *message = n1+n2;
  send(client_socket , message , strlen(message) , 0);
  printf("Message sent to server\n");

  // Receive data from the server
  char buffer[1024] = {0};
  int valread = read(client_socket , buffer, 1024);
  printf("Received message from server: %s\n", buffer);

  // Close the socket
  close(client_socket);

  return NULL;
}

int main(int argc, char const *argv[]) {
  int client_socket;
  struct sockaddr_in address;

  // Create a socket
  if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Connect to the server
  memset(&address, '0', sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  if (inet_pton(AF_INET, SERVER_ADDRESS, &address.sin_addr) <= 0) {
    perror("inet_pton failed");
    exit(EXIT_FAILURE);
  }
  if (connect(client_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("connect failed");
    exit(EXIT_FAILURE);
  }

  // Create a thread to send a request to the server
  pthread_t send_thread;
  if (pthread_create(&send_thread, NULL, send_request, &client_socket) != 0) {
    perror("pthread_create failed");
    exit(EXIT_FAILURE);
  }

  // Wait for the thread to finish
  if (pthread_join(send_thread, NULL) != 0) {
    perror("pthread_join failed");
    exit(EXIT_FAILURE);
  }

  return 0;
}
