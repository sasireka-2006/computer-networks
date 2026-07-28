int main()
{
    initializeTable();
    while(1)
    {
        printf("\n");
        printf("=====================================\n");
        printf("HOST TABLE MENU\n");
        printf("=====================================\n");
        printf("1.Display Host Table\n");
        printf("2.Add Host\n");
        printf("3.Update Host\n");
        printf("4.Delete Host\n");
        printf("5.Start Simulation\n");
        printf("\nEnter Choice : ");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
                displayTable();
                break;
            case 2:
                addHost();
                break;
            case 3:
                updateHost();
                break;
            case 4:
                deleteHost();
                break;
            case 5:
                goto simulation;
            default:
                printf("\nInvalid Choice\n");
        }
    }
simulation:
    displayTable();
        writeFile();
        readFile();
        textToBinary();
        printf("\nEnter Packet Size (Bits) : ");
        scanf("%d",&packetSize);
        createPackets();
        displayPackets();
        printf("\nEnter Frame Size (Bits) : ");
        scanf("%d",&frameSize);
        createFrames();
        displayFrames();
        printf("\nEnter Source URL : ");
        scanf("%s",sourceURL);
        printf("Enter Destination URL : ");
        scanf("%s",destinationURL);
        sourceIndex=findHost(sourceURL);
        destinationIndex=findHost(destinationURL);
        ipToBinary();
        macToBinary();
        generatePorts();
        selectProtocol();
        applicationLayer();
        transportLayer();
        networkLayer();
        dataLinkLayer();
        physicalLayer();
        printTransmission();
        printAll();
    return 0;
}
void parityGenerator()
{
    int i, j, ones, len;
    printf("\n=====================================\n");
    printf("        PARITY GENERATOR\n");
    printf("=====================================\n");
    for(i = 0; i < frameCount; i++)
    {
        ones = 0;
        len = strlen(frames[i]);
        strcpy(parityFrames[i], frames[i]);
        for(j = 0; j < len; j++)
        {
            if(frames[i][j] == '1')
                ones++;
        }
        if(ones % 2 == 0)
            strcat(parityFrames[i], "0");
        else
            strcat(parityFrames[i], "1");
        printf("\nFrame %d\n", i + 1);
        printf("Original : %s\n", frames[i]);
        printf("With Parity : %s\n", parityFrames[i]);
    }
}
void parityChecker()
{
    int i, j, ones, len;
    printf("\n=====================================\n");
    printf("         PARITY CHECKER\n");
    printf("=====================================\n");
    for(i = 0; i < frameCount; i++)
    {
        ones = 0;
        len = strlen(parityFrames[i]);
        for(j = 0; j < len; j++)
        {
            if(parityFrames[i][j] == '1')
                ones++;
        }
        printf("\nFrame %d : %s\n", i + 1, parityFrames[i]);

        if(ones % 2 == 0)
            printf("Status : No Error\n");
        else
            printf("Status : Error Detected\n");
    }
}
void printOriginalMessage()
{
    int i;
    char ch;

    printf("\n=========================================\n");
    printf("ORIGINAL MESSAGE AFTER BYTE DESTUFFING\n");
    printf("=========================================\n\n");

    for(i = 0; i < frameCount; i++)
    {
        ch = binaryToChar(destuffedFrames[i]);

        /* Ignore padding or null characters */
        if(ch != '\0')
            printf("%c", ch);
    }

    printf("\n");
}
char binaryToChar(char binary[])
{
    int i, value = 0;

    for(i = 0; i < 8; i++)
    {
        value = value * 2 + (binary[i] - '0');
    }

    return (char)value;
}
void dataLinkLayer()
{
    int i;
    int currentPacket=0;
        int choice;
        int frameNo, bitPos;
    printf("\n");
    printf("=====================================================\n");
    printf("                DATA LINK LAYER\n");
    printf("=====================================================\n");
    byteStuffing();
    byteDestuffing();
        parityGenerator();
        printf("\nDo you want to introduce an error? (1-Yes / 0-No): ");
scanf("%d", &choice);
if(choice == 1)
{
    printf("Enter Frame Number (1-%d): ", frameCount);
    scanf("%d", &frameNo);
    if(frameNo < 1 || frameNo > frameCount)
    {
        printf("\nInvalid Frame Number.\n");
    }
    else
    {
        printf("Enter Bit Position (0-%d): ",
               (int)strlen(parityFrames[frameNo-1]) - 1);
        scanf("%d", &bitPos);
        if(bitPos >= 0 &&
           bitPos < (int)strlen(parityFrames[frameNo-1]))
        {
            if(parityFrames[frameNo-1][bitPos] == '0')
                parityFrames[frameNo-1][bitPos] = '1';
            else
                parityFrames[frameNo-1][bitPos] = '0';
            printf("\nTransmission Error Introduced Successfully!\n");
            printf("Modified Frame : %s\n", parityFrames[frameNo-1]);
        }
        else
        {
            printf("\nInvalid Bit Position.\n");
        }
    }
}
        parityChecker();
    printf("\nFrame Size : %d Bits\n",frameSize);
    printf("Total Frames : %d\n",frameCount);
    printf("\nSource MAC\n");
    printf("%s\n",hosts[sourceIndex].mac);
    printf("\nDestination MAC\n");
    printf("%s\n",hosts[destinationIndex].mac);
    printf("\nSource MAC (Binary)\n");
    printf("%s\n",sourceMACBinary);
    printf("\nDestination MAC (Binary)\n");
    printf("%s\n",destinationMACBinary);
    printf("\n");
    for(i=0;i<frameCount;i++)
    {
        if(framePacketNumber[i]!=currentPacket)
        {
            currentPacket=framePacketNumber[i];
            printf("-----------------------------------------\n");
            printf("PACKET %d\n",currentPacket);
            printf("-----------------------------------------\n");
        }
        printf("\nFrame %d\n",i+1);
        printf("Original Frame  : %s\n",frames[i]);
        printf("Stuffed Frame   : %s\n",stuffedFrames[i]);
        printf("Destuffed Frame : %s\n",destuffedFrames[i]);
        printf("\n");
    }
}

=========================================
ORIGINAL MESSAGE AFTER BYTE DESTUFFING
=========================================

suba~}

Receiver Identified Protocol : CHAP
Receiver Output Saved in output2.txt

Byte Stuffing Output saved to output.txt


=====================================
        PARITY GENERATOR
=====================================

Frame 1
Original : 0111001
With Parity : 01110010

Frame 2
Original : 1011101
With Parity : 10111011

Frame 3
Original : 0101100
With Parity : 01011001

Frame 4
Original : 0100000
With Parity : 01000001

Frame 5
Original : 0110000
With Parity : 01100000

Frame 6
Original : 1011111
With Parity : 10111110

Frame 7
Original : 1001111
With Parity : 10011111

Frame 8
Original : 1010000
With Parity : 10100000

Do you want to introduce an error? (1-Yes / 0-No): 0

=====================================
         PARITY CHECKER
=====================================

Frame 1 : 01110010
Status : No Error

Frame 2 : 10111011
Status : No Error

Frame 3 : 01011001
Status : No Error

Frame 4 : 01000001
Status : No Error

Frame 5 : 01100000
Status : No Error

Frame 6 : 10111110
Status : No Error

Frame 7 : 10011111
Status : No Error

Frame 8 : 10100000
Status : No Error

Frame Size : 7 Bits
Total Frames : 8

Source MAC
AA:BB:CC:11:22:33

Destination MAC
10:20:30:40:50:60

Source MAC (Binary)
10101010 10111011 11001100 00010001 00100010 00110011

Destination MAC (Binary)
00010000 00100000 00110000 01000000 01010000 01100000

-----------------------------------------
PACKET 1
-----------------------------------------
Frame 1
Original Frame  : 0111001
Stuffed Frame   : 0111001
Destuffed Frame : 0111001
Frame 2
Original Frame  : 1011101
Stuffed Frame   : 1011101
Destuffed Frame : 1011101
Frame 3
Original Frame  : 0101100
Stuffed Frame   : 0101100
Destuffed Frame : 0101100
Frame 4
Original Frame  : 0100000
Stuffed Frame   : 0100000
Destuffed Frame : 0100000
-----------------------------------------
PACKET 2
-----------------------------------------
Frame 5
Original Frame  : 0110000
Stuffed Frame   : 0110000
Destuffed Frame : 0110000
Frame 6
Original Frame  : 1011111
Stuffed Frame   : 1011111
Destuffed Frame : 1011111
Frame 7
Original Frame  : 1001111
Stuffed Frame   : 1001111
Destuffed Frame : 1001111
Frame 8
Original Frame  : 1010000
Stuffed Frame   : 1010000
Destuffed Frame : 1010000
[24bcs152@mepcolinux EX2]$cat 1st.c
/*=========================================================
                        PART 1
    Structures, default table, global variables, constants
=========================================================*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAXDATA 5000
#define FRAMESIZE 32
#define MAXFRAME 100

struct table {
    char url[50];
    char ip[20];
    char mac[25];
};

struct table route[] = {
    {"www.google.com","192.168.10.1","00:11:22:33:44:55"},
    {"www.amazon.in","192.168.10.2","66:77:88:99:AA:BB"},
    {"www.mepco.edu","192.168.10.3","10:20:30:40:50:60"},
    {"www.yahoo.com","192.168.10.4","AA:BB:CC:DD:EE:FF"},
    {"www.microsoft.com","192.168.10.5","11:22:33:44:55:66"}
};
#define NUM_ROUTES (sizeof(route) / sizeof(route[0]))

struct frame {
    char soh[10];
    int frame_no;
    int length;
    char src_url[50];
    char dest_url[50];
    char src_ip[20];
    char dest_ip[20];
    char src_mac[25];
    char dest_mac[25];
    char data[FRAMESIZE + 1];
    char augmented[FRAMESIZE + 50];
    char crc[50];
    char codeword[FRAMESIZE + 50];
    char quotient[100]; // To store quotient for display
};

struct table src;
struct table dest;
struct frame f[MAXFRAME];

char srcurl[50];
char desturl[50];
char message[500];
char binary[MAXDATA];
char dataword[MAXDATA];
char generator[100];
char generator_binary[100];

int total_frames;
int binary_length;
int i;

// Function Declarations
void loadTable();
void getInput();
void searchAddress();
void readMessage();
void messageToBinary();
void polynomialToBinary();
void createFrames();
void sender();
void receiver();

/*=========================================================
                        PART 2
    Get source/destination URL, search table, validate URLs
=========================================================*/
void loadTable()
{
    printf("\n============================================================\n");
    printf("                       ROUTING TABLE\n");
    printf("============================================================\n");
    printf("%-20s %-20s %-20s\n", "URL", "IP ADDRESS", "MAC ADDRESS");
    printf("------------------------------------------------------------\n");
    for(i = 0; i < NUM_ROUTES; i++) {
        printf("%-20s %-20s %-20s\n", route[i].url, route[i].ip, route[i].mac);
    }
    printf("============================================================\n");
}

void getInput()
{
    printf("\nEnter Source URL : ");
    scanf("%s", srcurl);

    printf("Enter Destination URL : ");
    scanf("%s", desturl);

    printf("Enter Generator Polynomial : ");
    scanf(" %[^\n]", generator);
}

void searchAddress()
{
    int i, f1 = 0, f2 = 0;

    // Validate Source URL
    while(1) {
        f1 = 0;
        for(i = 0; i < NUM_ROUTES; i++) {
            if(strcmp(route[i].url, srcurl) == 0) {
                src = route[i];
                f1 = 1;
                break;
            }
        }
        if(f1 == 1) break;
        printf("\nSource URL Not Found. Enter Valid Source URL : ");
        scanf("%s", srcurl);
    }

    // Validate Destination URL
    while(1) {
        f2 = 0;
        for(i = 0; i < NUM_ROUTES; i++) {
            if(strcmp(route[i].url, desturl) == 0) {
                dest = route[i];
                f2 = 1;
                break;
            }
        }
        if(f2 == 1) break;
        printf("\nDestination URL Not Found. Enter Valid Destination URL : ");
        scanf("%s", desturl);
    }
}

/*=========================================================
                        PART 3
    Read message from file, ASCII to binary, decimal to binary
=========================================================*/
void readMessage()
{
    int ch;
    FILE *fp = fopen("message.txt", "r");

    if(fp == NULL) {
        printf("\nError: message.txt Not Found.");
        exit(0);
    }

    message[0] = '\0';
    while((ch = fgetc(fp)) != EOF) {
        int len = strlen(message);
        message[len] = ch;
        message[len + 1] = '\0';
    }
    fclose(fp);
    printf("\nMessage Read Successfully: %s\n", message);
}

void decimalToBinary(int num)
{
    int arr[8] = {0,0,0,0,0,0,0,0}; // Pre-fill with zeros
    int i = 0, j;
    int rem;
    char temp[2];

    // Manual paper logic as requested
    while(num > 0) {
        rem = num % 2;
        arr[i] = rem;
        num = num / 2;
        i++;
    }

    // Read array backwards to get correct binary order
    for(j = 7; j >= 0; j--) {
        temp[0] = arr[j] + '0';
        temp[1] = '\0';
        strcat(binary, temp);
    }
}

void messageToBinary()
{
    int i;
    binary[0] = '\0';

    for(i = 0; message[i] != '\0'; i++) {
        decimalToBinary((int)message[i]);
    }

    strcpy(dataword, binary);
    binary_length = strlen(dataword);
}

/*=========================================================
                        PART 4
    Polynomial to binary (manual power checking)
=========================================================*/
void polynomialToBinary()
{
    int power[20] = {0};
    int i = 0, max = 0, num = 0;

    while(generator[i] != '\0') {
        if(generator[i] == 'x' || generator[i] == 'X') {
            if(generator[i+1] == '^') {
                i += 2; num = 0;
                while(generator[i] >= '0' && generator[i] <= '9') {
                    num = num * 10 + (generator[i] - '0');
                    i++;
                }
                power[num] = 1;
                if(num > max) max = num;
            } else {
                power[1] = 1;
                if(max < 1) max = 1;
                i++;
            }
        }
        else if(generator[i] == '1') { power[0] = 1; i++; }
        else { i++; }
    }

    generator_binary[0] = '\0';
    for(i = max; i >= 0; i--) {
        if(power[i] == 1) strcat(generator_binary,"1");
        else strcat(generator_binary,"0");
    }
}

/*=========================================================
                        PART 5
    Create DDCMP-style frames, generate CRC using XOR
=========================================================*/
void crc_division(char *dividend, char *divisor, char *remainder, char *quotient)
{
    int d_len = strlen(dividend);
    int g_len = strlen(divisor);
    char temp[100];
    int i, j;

    strncpy(temp, dividend, g_len);
    temp[g_len] = '\0';
    quotient[0] = '\0';

    for (i = g_len - 1; i < d_len; i++) {
        if (i > g_len - 1) {
            int t_len = strlen(temp);
            temp[t_len] = dividend[i];
            temp[t_len + 1] = '\0';
        }

        if (temp[0] == '1') {
            strcat(quotient, "1");
            for (j = 1; j < g_len; j++) {
                temp[j] = (temp[j] == divisor[j]) ? '0' : '1';
            }
        } else {
            strcat(quotient, "0");
            for (j = 1; j < g_len; j++) {
                temp[j] = (temp[j] == '0') ? '0' : '1';
            }
        }

        for (j = 0; j < g_len - 1; j++) {
            temp[j] = temp[j + 1];
        }
        temp[g_len - 1] = '\0';
    }
    strcpy(remainder, temp);
}

void createFrames()
{
    int i, j, k = 0;
    int len = strlen(dataword);

    total_frames = len / FRAMESIZE;
    if(len % FRAMESIZE != 0) total_frames++;

    for(i = 0; i < total_frames; i++)
    {
        strcpy(f[i].soh, "01111110");
        f[i].frame_no = i + 1;
        strcpy(f[i].src_url, src.url);
        strcpy(f[i].src_ip, src.ip);
        strcpy(f[i].src_mac, src.mac);
        strcpy(f[i].dest_url, dest.url);
        strcpy(f[i].dest_ip, dest.ip);
        strcpy(f[i].dest_mac, dest.mac);

        int remaining = len - k;
        f[i].length = (remaining >= FRAMESIZE) ? FRAMESIZE : remaining;

        j = 0;
        while(j < FRAMESIZE) {
            if(k < len) {
                f[i].data[j] = dataword[k];
                k++;
            } else {
                f[i].data[j] = '0';
            }
            j++;
        }
        f[i].data[FRAMESIZE] = '\0';

        // Perform CRC Math inside Frame Creation
        int glen = strlen(generator_binary);
        strcpy(f[i].augmented, f[i].data);
        for(j = 0; j < glen - 1; j++) {
            strcat(f[i].augmented, "0");
        }

        crc_division(f[i].augmented, generator_binary, f[i].crc, f[i].quotient);

        strcpy(f[i].codeword, f[i].data);
        strcat(f[i].codeword, f[i].crc);
    }
}

/*=========================================================
                        PART 6
    Sender display, receiver verification, final main()
=========================================================*/
/*=========================================================
                        PART 6
    Sender display (Layered Output with DDCMP Frames)
=========================================================*/
void sender()
{
    printf("\n=======================================\n");
    printf("APPLICATION LAYER\n");
    printf("=======================================\n");
    printf("Application Layer Processing Completed.\n");

    printf("\n=======================================\n");
    printf("TRANSPORT LAYER\n");
    printf("=======================================\n");
    printf("Message          : %s\n", message);
    printf("Source Port      : 5000\n");
    printf("Destination Port : 8080\n");
    printf("Transport Layer Processing Completed.\n");

    printf("\n=======================================\n");
    printf("NETWORK LAYER\n");
    printf("=======================================\n");
    printf("Packets Created : %d\n", total_frames);
    printf("Network Layer Processing Completed.\n");

    printf("\n=======================================\n");
    printf("DATA LINK LAYER\n");
    printf("=======================================\n");

    for(i = 0; i < total_frames; i++) {
        printf("\n=========================================================\n");
        printf("                    DDCMP FRAME %d\n", f[i].frame_no);
        printf("=========================================================\n");

        printf("SOH : %s\n", f[i].soh);

        printf("\nHEADER");
        printf("\n---------------------------------------------------------");
        printf("\nSource URL      : %s", f[i].src_url);
        printf("\nDestination URL : %s", f[i].dest_url);
        printf("\nSource IP       : %s", f[i].src_ip);
        printf("\nDestination IP  : %s", f[i].dest_ip);
        printf("\nSource MAC      : %s", f[i].src_mac);
        printf("\nDestination MAC : %s", f[i].dest_mac);
        printf("\n---------------------------------------------------------");

        printf("\n\nSTX");

        printf("\n\nLENGTH          : %d bits", f[i].length);

        printf("\n\nDATA            :\n%s", f[i].data);

        printf("\n\nETX");

        printf("\n\nCRC (TRAILER)   : %s", f[i].crc);
        printf("\n=========================================================\n");
    }
    printf("\nData Link Layer Completed.\n");

    printf("\n=======================================\n");
    printf("SUMMARY\n");
    printf("=======================================\n");
    printf("Packets Created : %d\n", total_frames);
    printf("Frames Created  : %d\n", total_frames);
}

void receiver()
{
    int i, j;
    char remainder[50], quotient[100];
    int error_flag;
    char received_data[MAXDATA] = "";
    char ascii_char[9];
    int decimal_val;

    printf("\n=======================================\n");
    printf("       RECEIVER PROCESSING\n");
    printf("=======================================\n");

    for(i = 0; i < total_frames; i++) {
        printf("\n--- Receiving Frame %d ---\n", f[i].frame_no);
        printf("Received Codeword : %s\n", f[i].codeword);

        crc_division(f[i].codeword, generator_binary, remainder, quotient);

        printf("Decoder Quotient  : %s\n", quotient);
        printf("Syndrome (Rem)    : %s\n", remainder);

        error_flag = 0;
        for(j = 0; j < strlen(remainder); j++) {
            if(remainder[j] == '1') {
                error_flag = 1;
                break;
            }
        }

        if(error_flag == 1) {
            printf("Status: ERROR DETECTED. Frame %d discarded.\n", f[i].frame_no);
        } else {
            printf("Status: NO ERROR (Zero syndrome). Frame %d accepted.\n", f[i].frame_no);
            strncat(received_data, f[i].data, f[i].length);
        }
    }

    printf("\nExtracted Binary Data:\n%s\n", received_data);
    printf("\nDecoded Message: ");

    for(i = 0; i < strlen(received_data); i += 8) {
        strncpy(ascii_char, &received_data[i], 8);
        ascii_char[8] = '\0';
        decimal_val = 0;
        for(j = 0; j < 8; j++) {
            decimal_val = decimal_val * 2 + (ascii_char[j] - '0');
        }
        printf("%c", decimal_val);
    }
    printf("\n\n");
}

int main()
{
    loadTable();
    getInput();
    searchAddress();
    readMessage();
    messageToBinary();
    polynomialToBinary();
    createFrames();
    sender();
    receiver();

    return 0;
}
[24bcs152@mepcolinux ex2]$cc 1st.c
[24bcs152@mepcolinux ex2]$./a.out

============================================================
                       ROUTING TABLE
============================================================
URL                  IP ADDRESS           MAC ADDRESS
------------------------------------------------------------
www.google.com       192.168.10.1         00:11:22:33:44:55
www.amazon.in        192.168.10.2         66:77:88:99:AA:BB
www.mepco.edu        192.168.10.3         10:20:30:40:50:60
www.yahoo.com        192.168.10.4         AA:BB:CC:DD:EE:FF
www.microsoft.com    192.168.10.5         11:22:33:44:55:66
============================================================

Enter Source URL : www.google.com
Enter Destination URL : www.yahoo.com
Enter Generator Polynomial : x^3+x+1

Message Read Successfully: I LOVE INDIA


=======================================
APPLICATION LAYER
=======================================
Application Layer Processing Completed.

=======================================
TRANSPORT LAYER
=======================================
Message          : I LOVE INDIA

Source Port      : 5000
Destination Port : 8080
Transport Layer Processing Completed.

=======================================
NETWORK LAYER
=======================================
Packets Created : 4
Network Layer Processing Completed.

=======================================
DATA LINK LAYER
=======================================

=========================================================
                    DDCMP FRAME 1
=========================================================
SOH : 01111110

HEADER
---------------------------------------------------------
Source URL      : www.google.com
Destination URL : www.yahoo.com
Source IP       : 192.168.10.1
Destination IP  : 192.168.10.4
Source MAC      : 00:11:22:33:44:55
Destination MAC : AA:BB:CC:DD:EE:FF
---------------------------------------------------------

STX

LENGTH          : 32 bits

DATA            :
01001001001000000100110001001111

ETX

CRC (TRAILER)   : 001
=========================================================

=========================================================
                    DDCMP FRAME 2
=========================================================
SOH : 01111110

HEADER
---------------------------------------------------------
Source URL      : www.google.com
Destination URL : www.yahoo.com
Source IP       : 192.168.10.1
Destination IP  : 192.168.10.4
Source MAC      : 00:11:22:33:44:55
Destination MAC : AA:BB:CC:DD:EE:FF
---------------------------------------------------------

STX

LENGTH          : 32 bits

DATA            :
01010110010001010010000001001001

ETX

CRC (TRAILER)   : 001
=========================================================

=========================================================
                    DDCMP FRAME 3
=========================================================
SOH : 01111110

HEADER
---------------------------------------------------------
Source URL      : www.google.com
Destination URL : www.yahoo.com
Source IP       : 192.168.10.1
Destination IP  : 192.168.10.4
Source MAC      : 00:11:22:33:44:55
Destination MAC : AA:BB:CC:DD:EE:FF
---------------------------------------------------------

STX

LENGTH          : 32 bits

DATA            :
01001110010001000100100101000001

ETX

CRC (TRAILER)   : 100
=========================================================

=========================================================
                    DDCMP FRAME 4
=========================================================
SOH : 01111110

HEADER
---------------------------------------------------------
Source URL      : www.google.com
Destination URL : www.yahoo.com
Source IP       : 192.168.10.1
Destination IP  : 192.168.10.4
Source MAC      : 00:11:22:33:44:55
Destination MAC : AA:BB:CC:DD:EE:FF
---------------------------------------------------------

STX

LENGTH          : 8 bits

DATA            :
00001010000000000000000000000000

ETX

CRC (TRAILER)   : 101
=========================================================

Data Link Layer Completed.

=======================================
SUMMARY
=======================================
Packets Created : 4
Frames Created  : 4

=======================================
       RECEIVER PROCESSING
=======================================

--- Receiving Frame 1 ---
Received Codeword : 01001001001000000100110001001111001
Decoder Quotient  : 01010110011100101011011101111111
Syndrome (Rem)    : 000
Status: NO ERROR (Zero syndrome). Frame 1 accepted.

--- Receiving Frame 2 ---
Received Codeword : 01010110010001010010000001001001001
Decoder Quotient  : 01001100111000010101110011101111
Syndrome (Rem)    : 000
Status: NO ERROR (Zero syndrome). Frame 2 accepted.

--- Receiving Frame 3 ---
Received Codeword : 01001110010001000100100101000001100
Decoder Quotient  : 01010000010110010010010011100100
Syndrome (Rem)    : 000
Status: NO ERROR (Zero syndrome). Frame 3 accepted.

--- Receiving Frame 4 ---
Received Codeword : 00001010000000000000000000000000101
Decoder Quotient  : 00001001011100101110010111001011
Syndrome (Rem)    : 000
Status: NO ERROR (Zero syndrome). Frame 4 accepted.

Extracted Binary Data:
01001001001000000100110001001111010101100100010100100000010010010100111001000100010010010100000100001010

Decoded Message: I LOVE INDIA
exit
