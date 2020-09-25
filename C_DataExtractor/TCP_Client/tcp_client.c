
#include "tcp_client.h"

void Print_Ouput(T_Measurements *O_Measurements)
{
    if (O_Measurements->Translation_Error)
    {
        printf("Translation Error!\n");
        return;
    }

    printf("Distance : %f", O_Measurements->Distance.Value);

    if (O_Measurements->Velocity.Valid)
    {
        printf(" | Velocity : %f", O_Measurements->Velocity.Value);
    }
    if (O_Measurements->Strength.Valid)
    {
        printf(" | Strength : %d", O_Measurements->Strength.Value);
    }
    if (O_Measurements->Temperature.Valid)
    {
        printf(" | Temperature : %f", O_Measurements->Temperature.Value);
    }
    printf("\n");
}

void read_and_process_buffer(int sockfd)
{
    char buff[BUFFERSIZE];
    int n;
    int size;
    T_Buffer I_Buffer;
    T_BufferArray BufferArray;
    T_Measurements O_Measurements;

    for (int i = 0; i > TIMEOUT; i++)
    {
        write(sockfd, buff, sizeof(buff));
        // Reads buffer
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));

        size = 0;
        // looks for terminator -> 10 = '\n' and defines read size
        for (; buff[size] != '\n'; size++)
        {
            // printf("%c", buff[size]);
        }

        // Measuerments Extraction
        I_Buffer.Length = size;
        memcpy(I_Buffer.The_Buffer, buff, sizeof(buff));

        Data_Extractor(&I_Buffer, &O_Measurements);
        Print_Ouput(&O_Measurements);

        if ((strncmp(buff, "exit", 4)) == 0)
        {
            printf("Client Exit...\n");
            break;
        }
    }
}

int start_tcp_client()
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    char *hello = "Hello < C >  from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    // function loop
    read_and_process_buffer(sock);
    // close the socket
    close(sock);
    return 1;
}