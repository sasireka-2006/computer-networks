#include <stdio.h>
#include <string.h>
#include <unistd.h>      // Replaces closesocket with close()
#include <arpa/inet.h>   // For htons and network addresses
#include <sys/socket.h>  // Standard Linux socket headers

#define PORT 5000

// Converts a text string into a binary string of '0's and '1's
void textToBinary(char text[], char binaryOutput[])
{
    int i, j;
    int index = 0;

    for (i = 0; text[i] != '\0'; i++)
    {
        // Extract bits from MSB to LSB for each character
        for (j = 7; j >= 0; j--)
        {
            if ((text[i] >> j) & 1)
                binaryOutput[index++] = '1';
            else
                binaryOutput[index++] = '0';
        }
    }
    binaryOutput[index] = '\0';
}

// Bit stuffing function
void bitStuff(char input[], char output[])
{
    int i;
    int count = 0;
    int j = 0;

    for (i = 0; input[i] != '\0'; i++)
    {
        output[j] = input[i];
        j++;

        if (input[i] == '1')
            count++;
        else
            count = 0;

        // Insert 0 after five consecutive 1s
        if (count == 5)
        {
            output[j] = '0';
            j++;
            count = 0;
        }
    }

    output[j] = '\0';
}

// Checksum function
int checksum(char data[])
{
    int i;
    int sum = 0;

    for (i = 0; data[i] != '\0'; i++)
    {
        sum = sum + (data[i] - '0');
    }

    return (255 - sum) & 255;
}

int main()
{
    int sock;
    int len;
    int check;
    socklen_t client_len; // Linux uses socklen_t for address sizes

    char message[100];      // Stores received text string
    char binaryStr[1000];   // Stores text converted to binary (8 bits per char)
    char stuffed[2000];     // Stores stuffed binary data

    struct sockaddr_in server;
    struct sockaddr_in client;

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) // Linux sockets return -1 on error
    {
        printf("Socket creation failed.\n");
        return 1;
    }

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Bind failed.\n");
        close(sock);
        return 1;
    }

    printf("Server is waiting...\n");

    // Receive string from client
    client_len = sizeof(client);

    len = recvfrom(sock, message, sizeof(message) - 1, 0,
                   (struct sockaddr *)&client, &client_len);

    if (len < 0)
    {
        printf("Receive failed.\n");
        close(sock);
        return 1;
    }

    message[len] = '\0';

    printf("String received: %s\n", message);

    // 1. Convert text to binary string
    textToBinary(message, binaryStr);

    // 2. Bit stuffing on the converted binary string
    bitStuff(binaryStr, stuffed);

    printf("After bit stuffing: %s\n", stuffed);

    // Calculate checksum AFTER bit stuffing
    check = checksum(stuffed);

    printf("Checksum: %d\n", check);

    // Send stuffed data
    sendto(sock, stuffed, strlen(stuffed), 0,
           (struct sockaddr *)&client, client_len);

    // Send checksum
    sendto(sock, (char *)&check, sizeof(check), 0,
           (struct sockaddr *)&client, client_len);

    printf("Stuffed data and checksum sent to client.\n");

    // Close socket
    close(sock); // Replaces closesocket()

    return 0;
}
