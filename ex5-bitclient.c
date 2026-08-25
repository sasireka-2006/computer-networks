#include <stdio.h>
#include <string.h>
#include <unistd.h>      // Replaces closesocket with close()
#include <arpa/inet.h>   // For htons and inet_addr
#include <sys/socket.h>  // Standard Linux socket headers

#define PORT 5000

// Bit de-stuffing function
void bitDestuff(char input[], char output[])
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

        // Skip the 0 inserted after five consecutive 1s
        if (count == 5)
        {
            i++;
            count = 0;
        }
    }

    output[j] = '\0';
}

// Converts binary string ('0' and '1') back to original text string
void binaryToText(char binaryInput[], char textOutput[])
{
    int i, j;
    int len = strlen(binaryInput);
    int index = 0;

    // Process bits in groups of 8 (1 byte)
    for (i = 0; i + 7 < len; i += 8)
    {
        char ch = 0;
        for (j = 0; j < 8; j++)
        {
            ch = (ch << 1) | (binaryInput[i + j] - '0');
        }
        textOutput[index++] = ch;
    }
    textOutput[index] = '\0';
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
    int receivedChecksum;
    int calculatedChecksum;
    char choice;
    int position;

    char message[100];       // Stores text input from user
    char stuffed[1000];      // Expanded size for large binary streams
    char destuffed[1000];    // Expanded size for large binary streams
    char textMessage[100];   // Stores final converted readable text

    struct sockaddr_in server;

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
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Get string from user (Normal Text String)
    printf("Enter text string: ");
    fgets(message, sizeof(message), stdin);

    // Remove newline
    message[strcspn(message, "\n")] = '\0';

    // Send text string to server
    sendto(sock, message, strlen(message), 0,
           (struct sockaddr *)&server, sizeof(server));

    printf("Text string sent to server.\n");

    // Receive stuffed data
    len = recvfrom(sock, stuffed, sizeof(stuffed) - 1, 0,
                   NULL, NULL);

    if (len < 0)
    {
        printf("Receive failed.\n");
        close(sock);
        return 1;
    }

    stuffed[len] = '\0';

    // Receive checksum
    recvfrom(sock, (char *)&receivedChecksum,
             sizeof(receivedChecksum), 0,
             NULL, NULL);

    printf("\nReceived stuffed data: %s\n", stuffed);
    printf("Received checksum: %d\n", receivedChecksum);

    // Ask user whether to introduce error
    printf("\nDo you want to introduce an error? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y')
    {
        printf("Enter bit position to flip (starting from 0): ");
        scanf("%d", &position);

        if (position >= 0 && position < (int)strlen(stuffed))
        {
            // Flip the selected bit
            if (stuffed[position] == '0')
                stuffed[position] = '1';
            else
                stuffed[position] = '0';

            printf("Modified stuffed data: %s\n", stuffed);
        }
        else
        {
            printf("Invalid position.\n");
        }
    }
    else
    {
        printf("No error introduced.\n");
    }

    // De-stuff the received data
    bitDestuff(stuffed, destuffed);
    printf("After de-stuffing (Binary): %s\n", destuffed);

    // Calculate checksum again on stuffed data
    calculatedChecksum = checksum(stuffed);
    printf("Calculated checksum: %d\n", calculatedChecksum);

    // Compare checksum
    if (calculatedChecksum == receivedChecksum)
    {
        printf("\nChecksum matched!\n");
        printf("Data is CORRECT.\n");

        // Convert the clean binary back into readable text string
        binaryToText(destuffed, textMessage);
        printf("Decoded Text Message: %s\n", textMessage);
    }
    else
    {
        printf("\nChecksum mismatch!\n");
        printf("ERROR DETECTED. (Cannot decode corrupt binary correctly)\n");
    }

    // Close socket
    close(sock); // Replaces closesocket()

    return 0;
}
