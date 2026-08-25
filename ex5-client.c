#include <stdio.h>
#include <string.h>
#include <unistd.h>      // Replaces closesocket with close()
#include <arpa/inet.h>   // For htons and inet_addr
#include <sys/socket.h>  // Standard Linux socket headers

// Must match the server's unique port perfectly!
#define PORT 5056

int main()
{
    int sock;
    int len;

    char message[100];
    char buffer[100];

    struct sockaddr_in server;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Connect to localhost

    // Get string from user
    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);

    // Remove newline
    message[strcspn(message, "\n")] = '\0';

    // Send string to server
    sendto(sock, message, strlen(message), 0,
           (struct sockaddr *)&server, sizeof(server));

    printf("Message sent to server.\n");

    // Receive the message back from the server
    len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                   NULL, NULL);

    if (len < 0)
    {
        printf("Receive failed.\n");
        close(sock);
        return 1;
    }

    buffer[len] = '\0';

    // Print the output received from the server
    printf("Message received back from server: %s\n", buffer);

    // Close socket cleanly
    close(sock);
    return 0;
}
