#include <stdio.h>
#include <string.h>
#include <unistd.h>      // Replaces closesocket with close()
#include <arpa/inet.h>   // For htons and network addresses
#include <sys/socket.h>  // Standard Linux socket headers

// Changed port to 5056 so other students' programs don't block you!
#define PORT 5056

int main()
{
    int sock;
    int len;
    socklen_t client_len; // Linux uses socklen_t for address sizes
    int opt = 1;          // For setsockopt

    char message[100];

    struct sockaddr_in server;
    struct sockaddr_in client;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) // Linux sockets return -1 on error
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    // Forces immediate reuse of port if locked in background
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        printf("Setsockopt failed.\n");
        close(sock);
        return 1;
    }

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Bind failed on Port %d.\n", PORT);
        close(sock);
        return 1;
    }

    printf("Server is waiting on Port %d...\n", PORT);

    // Receive message from client (Executes only once, then closes)
    client_len = sizeof(client);

    len = recvfrom(sock, message, sizeof(message) - 1, 0,
                   (struct sockaddr *)&client, &client_len);

    if (len < 0)
    {
        printf("Receive failed.\n");
        close(sock);
        return 1;
    }

    // Add null character
    message[len] = '\0';

    printf("Message received: %s\n", message);

    // Send same message back to client
    sendto(sock, message, len, 0,
           (struct sockaddr *)&client, client_len);

    printf("Message sent back to client.\n");

    // Close socket
    close(sock);
    printf("Server shut down successfully.\n");

    return 0;
}
