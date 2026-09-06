#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Structure for client
struct ChatClient
{
    int socket;
    struct sockaddr_in server_address;
    char message[BUFFER_SIZE];
};

// Function declarations
int createClientSocket();
void connectToServer(struct ChatClient *client);
void startChat(struct ChatClient *client);

int main()
{
    struct ChatClient client;

    // Create socket
    client.socket = createClientSocket();

    // Connect to server
    connectToServer(&client);

    printf("====================================\n");
    printf("          CHAT CLIENT\n");
    printf("====================================\n");
    printf("Connected to server.\n");
    printf("Type 'exit' to end the chat.\n\n");

    // Start chat
    startChat(&client);

    close(client.socket);

    printf("Connection closed.\n");

    return 0;
}

// Create socket
int createClientSocket()
{
    int client_socket;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    return client_socket;
}

// Connect to server
void connectToServer(struct ChatClient *client)
{
    client->server_address.sin_family = AF_INET;
    client->server_address.sin_port = htons(PORT);

    // Server is running on localhost
    client->server_address.sin_addr.s_addr =
        inet_addr("127.0.0.1");

    if (connect(
        client->socket,
        (struct sockaddr *)&client->server_address,
        sizeof(client->server_address)
    ) < 0)
    {
        perror("Connection failed");
        close(client->socket);
        exit(1);
    }
}

// Chat function
void startChat(struct ChatClient *client)
{
    while (1)
    {
        // Get message from client
        printf("Client: ");

        fgets(
            client->message,
            BUFFER_SIZE,
            stdin
        );

        // Send message to server
        send(
            client->socket,
            client->message,
            strlen(client->message),
            0
        );

        // Exit condition
        if (strncmp(client->message, "exit", 4) == 0)
        {
            break;
        }

        // Clear buffer
        memset(client->message, 0, BUFFER_SIZE);

        // Receive server message
        int bytes_received = recv(
            client->socket,
            client->message,
            BUFFER_SIZE - 1,
            0
        );

        if (bytes_received <= 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        client->message[bytes_received] = '\0';

        printf("Server: %s", client->message);

        // Exit condition
        if (strncmp(client->message, "exit", 4) == 0)
        {
            break;
        }
    }
}