#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// Changed port to 5056 to keep your process safe from port conflicts
#define PORT 5056

int main()
{
    int sock;
    int len;
    int i;
    socklen_t client_len;
    int opt = 1;

    char str[100];
    char temp;

    struct sockaddr_in server;
    struct sockaddr_in client;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    // Reuse option to avoid immediate lockouts
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        printf("Setsockopt failed.\n");
        close(sock);
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Bind failed.\n");
        close(sock);
        return 1;
    }

    printf("Server is waiting on Port %d...\n", PORT);
    fflush(stdout); // Forces terminal to show text instantly

    client_len = sizeof(client);

    len = recvfrom(sock, str, sizeof(str) - 1, 0,
                   (struct sockaddr *)&client, &client_len);

    if (len < 0)
    {
        printf("Receive failed.\n");
        close(sock);
        return 1;
    }

    str[len] = '\0';

    printf("String received: %s\n", str);

    // Reverse the string
    for (i = 0; i < len / 2; i++)
    {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }

    printf("Reversed string: %s\n", str);

    sendto(sock, str, strlen(str), 0,
           (struct sockaddr *)&client, client_len);

    printf("Reversed string sent to client.\n");

    fflush(stdout); // Forces the last logs out before closing

    close(sock);
    return 0;
}
