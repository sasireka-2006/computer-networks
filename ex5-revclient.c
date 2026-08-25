#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5056

int main()
{
    int sock;
    int len;

    char str[100];
    char buffer[100];

    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter string: ");
    fgets(str, sizeof(str), stdin);

    str[strcspn(str, "\n")] = '\0';

    sendto(sock, str, strlen(str), 0,
           (struct sockaddr *)&server, sizeof(server));

    printf("String sent to server.\n");

    len = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                   NULL, NULL);

    if (len < 0)
    {
        printf("Receive failed.\n");
        close(sock);
        return 1;
    }

    buffer[len] = '\0';

    printf("Reversed string from server: %s\n", buffer);

    close(sock);
    return 0;
}
