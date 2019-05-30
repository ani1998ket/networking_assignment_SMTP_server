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

int sockaddr_in_len = sizeof(struct sockaddr_in);

void init_socket(struct socket_data_struct * socket_data){
     if((socket_data->descriptor = socket(AF_INET, SOCK_STREAM, 0)) == ERROR){
        perror("server socket :");
        exit(-1);
    }
}
void init_socket_address(struct socket_data_struct * socket_data, short port, in_addr_t ip_adress){
    (socket_data->address).sin_family = AF_INET;
    (socket_data->address).sin_port = htons(port);
    (socket_data->address).sin_addr.s_addr = ip_adress;
    bzero(&(socket_data->address).sin_zero, 8);
}
void bind_socket(struct socket_data_struct * socket_data){

    if(bind((socket_data->descriptor), (struct sockaddr *)&(socket_data->address), sizeof(socket_data->address)) == ERROR){
        perror("bind :");
        exit(-1);
    }
}
void listen_socket(struct socket_data_struct * socket_data){

    if(listen(socket_data->descriptor, MAX_CLIENTS) == ERROR){
        perror("listen");
        exit(-1);
    }

}
void accept_socket(struct socket_data_struct * server_socket_data, struct socket_data_struct * client_socket_data){
    
     if((client_socket_data->descriptor = accept(server_socket_data->descriptor, (struct sockaddr *)&(client_socket_data->address), &(sockaddr_in_len))) == ERROR){
        perror("accept");
        exit(-1);
    }
}

void connect_socket(struct socket_data_struct * socket_data){
    if(connect(socket_data->descriptor, (struct sockaddr *)&socket_data->address, sockaddr_in_len) == ERROR){
        perror("connect :");
        exit(-1);
    }
}