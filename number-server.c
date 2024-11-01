#include "http-server.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int num = 0; // Global variable to hold the number

// Function prototypes
void handle_404(int client_sock, char *path);
void handle_shownum(int client_sock);
void handle_increment(int client_sock);
void handle_add(int client_sock, const char *query);

// Implement handle_404
void handle_404(int client_sock, char *path) {
    printf("SERVER LOG: Got request for unrecognized path \"%s\"\n", path);
    const char *header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
    char response_body[BUFFER_SIZE];
    snprintf(response_body, BUFFER_SIZE, "Error 404: Unrecognized path \"%s\"\n", path);
    
    write(client_sock, header, strlen(header));
    write(client_sock, response_body, strlen(response_body));
}

void handle_shownum(int client_sock) {
    const char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    char response_body[BUFFER_SIZE];
    snprintf(response_body, BUFFER_SIZE, "Your number is %d\n", num);
    
    write(client_sock, header, strlen(header));
    write(client_sock, response_body, strlen(response_body));
}

void handle_increment(int client_sock) {
    num++; // Increment the global number
    const char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    char response_body[BUFFER_SIZE];
    snprintf(response_body, BUFFER_SIZE, "Your number is now %d\n", num);
    
    write(client_sock, header, strlen(header));
    write(client_sock, response_body, strlen(response_body));
}

void handle_add(int client_sock, const char *query) {
    int value = 0;
    sscanf(query, "value=%d", &value); // Extract the value from the query string
    num += value; // Add the value to the global number
    const char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    char response_body[BUFFER_SIZE];
    snprintf(response_body, BUFFER_SIZE, "Your number is now %d\n", num);
    
    write(client_sock, header, strlen(header));
    write(client_sock, response_body, strlen(response_body));
}

void handle_response(char *request, int client_sock) {
    char path[256];
    printf("\nSERVER LOG: Got request: \"%s\"\n", request);

    // Parse the path out of the request line
    if (sscanf(request, "GET %255s", path) != 1) {
        printf("Invalid request line\n");
        return;
    }

    if (strcmp(path, "/shownum") == 0) {
        handle_shownum(client_sock);
    } else if (strcmp(path, "/increment") == 0) {
        handle_increment(client_sock);
    } else if (strncmp(path, "/add?", 5) == 0) {
        handle_add(client_sock, path + 5); // Pass the query string
    } else {
        handle_404(client_sock, path); // Handle unknown paths
    }
}

int main(int argc, char *argv[]) {
    int port = 0;
    if (argc >= 2) { // If called with a port number, use that
        port = atoi(argv[1]);
    }
    start_server(&handle_response, port);
}
