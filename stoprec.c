#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 1024
int checksum(char *data)
{
    int sum = 0;
    int i;
    for (i = 0; data[i] != '\0'; i++)
        sum += (unsigned char)data[i];
    return sum % 256;
}
int main(int argc, char *argv[])
{
    int sockfd;
    int port;
    struct sockaddr_in receiver, sender;
    socklen_t sender_len;
    char buffer[BUFFER_SIZE];
    char data[500];
    char response[100];
    int seq_no;
    int received_checksum;
    int expected_seq;
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }
    port = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        return 1;
    }
    sender_len = sizeof(sender);
    receiver.sin_family = AF_INET;
    receiver.sin_addr.s_addr = INADDR_ANY;
    receiver.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&receiver, sizeof(receiver)) < 0)
    {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }
    printf("\n====================================\n");
    printf(" STOP AND WAIT ARQ\n");
    printf(" RECEIVER\n");
    printf("====================================\n");
    printf("Waiting on Port: %d\n\n", port);
    expected_seq = 0;
    while (1)
    {
        int bytes;
        int calculated_checksum;
        memset(buffer, 0, sizeof(buffer));
        memset(data, 0, sizeof(data));
        bytes = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&sender, &sender_len);
        if (bytes < 0)
            continue;
        if (strcmp(buffer, "END") == 0)
        {
            printf("\n====================================\n");
            printf("Communication completed.\n");
            printf("====================================\n");
            break;
        }
        if (sscanf(buffer, "%d:%499[^:]:%d",
                   &seq_no, data, &received_checksum) != 3)
            continue;
        printf("\nReceived Frame : %d\n", seq_no);
        printf("Data : %s\n", data);
        calculated_checksum = checksum(data);
        printf("Received Checksum : %d\n", received_checksum);
        printf("Calculated Checksum : %d\n", calculated_checksum);
        if (received_checksum != calculated_checksum)
        {
            printf("Result : Data is corrupted.\n");
            sprintf(response, "NAK:%d", seq_no);
            sendto(sockfd, response, strlen(response), 0,
                   (struct sockaddr *)&sender, sender_len);
            printf("Sent : %s\n", response);
            continue;
        }
        printf("Result : Checksum is correct.\n");
        if (seq_no == expected_seq)
        {
            int ack_no = 1 - seq_no;
            printf("Action : Frame %d accepted.\n", seq_no);
            sprintf(response, "ACK:%d", ack_no);
            sendto(sockfd, response, strlen(response), 0,
                   (struct sockaddr *)&sender, sender_len);
            printf("Sent : %s\n", response);
            expected_seq = 1 - expected_seq;
        }
        else
        {
            int ack_no = 1 - seq_no;
            printf("Action : Duplicate frame %d.\n", seq_no);
            sprintf(response, "ACK:%d", ack_no);
            sendto(sockfd, response, strlen(response), 0,
                   (struct sockaddr *)&sender, sender_len);
            printf("Sent again : %s\n", response);
        }
    }
    close(sockfd);
    return 0;
}
