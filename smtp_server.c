#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<error.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

#include "basic_socket_functions.h"
#include "smtp_basic_commands.h"
#define PORT 10000

int main(int argc, char *argv[]){
    struct socket_data_struct server_data, client_data;

    init_socket(&server_data);
    init_socket_address(&server_data, atoi(argv[1]), INADDR_ANY);
    bind_socket(&server_data);
    listen_socket(&server_data);

    while(1){
        accept_socket(&server_data, &client_data);

        init_SMTP(&server_data, &client_data);
        process_RSET();
        
        // client_data.data_len = recv(client_data.descriptor, client_data.data, MAX_BUFFER_SIZE, 0);
        client_data.data_len = 1;
        while(client_data.data_len){
            client_data.data_len = recv(client_data.descriptor, client_data.data, 50, 0);
            client_data.data[client_data.data_len] = '\0';

            process_client_commands(&server_data, &client_data);
        }
        close(client_data.descriptor);
    }
    close(server_data.descriptor);
}
