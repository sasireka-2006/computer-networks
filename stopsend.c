#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <time.h>
#define BUFFER_SIZE 1024
#define TIMEOUT 5
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
    int n;
    struct sockaddr_in receiver;
    socklen_t receiver_len;
    char data[500];
    char frame[BUFFER_SIZE];
    char response[100];
    int choice;
    int seq_no;
    int i;
    if (argc != 3)
    {
        printf("Usage: %s <receiver_ip> <port>\n", argv[0]);
        return 1;
    }
    port = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        return 1;
    }
    receiver_len = sizeof(receiver);
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(port);
    if (inet_pton(AF_INET, argv[1], &receiver.sin_addr) <= 0)
    {
        printf("Invalid IP address\n");
        close(sockfd);
        return 1;
    }
    printf("\n====================================\n");
    printf(" STOP AND WAIT ARQ\n");
    printf(" SENDER\n");
    printf("====================================\n");
    printf("Receiver IP : %s\n", argv[1]);
    printf("Port : %d\n", port);
    printf("Timeout : %d seconds\n", TIMEOUT);
    printf("====================================\n\n");
    printf("Enter number of frames: ");
    if (scanf("%d", &n) != 1)
        return 1;
    getchar();
    seq_no = 0;
    for (i = 0; i < n; i++)
    {
        int first_attempt = 1;
        printf("\nEnter data for Frame %d (Sequence %d): ",
               i, seq_no);
        if (!fgets(data, sizeof(data), stdin))
            break;
        data[strcspn(data, "\n")] = '\0';
        while (1)
        {
            int cs = checksum(data);
            fd_set readfds;
            struct timeval tv;
            int result;
            int ack_no;
            if (first_attempt)
            {
                printf("\n--- Choose an option ---\n");
                printf("1. Send normally\n");
                printf("2. Send corrupted frame\n");
                printf("3. Do not send frame\n");
                printf("Enter choice (1-3): ");
                if (scanf("%d", &choice) != 1)
                    choice = 1;
                getchar();
                first_attempt = 0;
            }
            else
            {
                choice = 1;
            }
            if (choice == 2)
            {
                printf("[TEST] Changing checksum...\n");
                cs = (cs + 15) % 256;
            }
            sprintf(frame, "%d:%s:%d", seq_no, data, cs);
            if (choice == 3)
            {
                printf("[TEST] Frame %d was not sent.\n", seq_no);
            }
            else
            {
                printf("Sending Frame %d...\n", seq_no);
                sendto(sockfd, frame, strlen(frame), 0,
                       (struct sockaddr *)&receiver, receiver_len);
            }
            FD_ZERO(&readfds);
            FD_SET(sockfd, &readfds);
            tv.tv_sec = TIMEOUT;
            tv.tv_usec = 0;
            printf("Waiting for ACK...\n");
            result = select(sockfd + 1, &readfds, NULL, NULL, &tv);
            if (result == 0)
            {
                printf("--> Timeout! No ACK received.\n");
                printf("--> Sending Frame %d again...\n\n", seq_no);
                continue;
            }
            memset(response, 0, sizeof(response));
            recvfrom(sockfd, response, sizeof(response) - 1, 0,
                     (struct sockaddr *)&receiver, &receiver_len);
            if (sscanf(response, "%*[^:]:%d", &ack_no) != 1)
                continue;
            if (strncmp(response, "ACK:", 4) == 0)
            {
                int expected_ack = 1 - seq_no;
                if (ack_no == expected_ack)
                {
                    printf("ACK %d received.\n", ack_no);
                    printf("Frame %d completed.\n", seq_no);
                    seq_no = 1 - seq_no;
                    break;
                }
                else
                {
                    printf("Wrong ACK %d received.\n", ack_no);
                    printf("Expected ACK %d.\n", expected_ack);
                }
            }
            else if (strncmp(response, "NAK:", 4) == 0)
            {
                printf("NAK %d received.\n", ack_no);
                printf("Frame has an error. Sending again...\n\n");
            }
        }
    }
    printf("\n====================================\n");
    printf("All frames sent successfully.\n");
    printf("====================================\n");
    sendto(sockfd, "END", strlen("END"), 0,
           (struct sockaddr *)&receiver, receiver_len);
    close(sockfd);
    return 0;
}
