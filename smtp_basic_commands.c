#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#include "basic_socket_functions.h"
#include "smtp_basic_commands.h"

#define MAX_RECEIVERS 50
char * clientIP, mailFrom[MAX_BUFFER_SIZE], email_data[MAX_BUFFER_SIZE*4];
char * domain_name, * user_name;

char ** receivers;

int no_receivers = 0;
char * substring;

int user_is_valid = 0;
char * temp_pointer;

FILE *fptr;

int findCommand(char * command, char * data){
    if((strstr(data, command)) != NULL){
       return 1;
    }
    return 0;
}

void send_data(short client, char * buffer ){
    send(client, buffer, MAX_BUFFER_SIZE, 0);
}
int init_SMTP(struct socket_data_struct * server_data, struct socket_data_struct * client_data){
    printf("New client connected from port no %d and IP %s\n", ntohs(client_data->address.sin_port), inet_ntoa(client_data->address.sin_addr));
    strncpy(server_data->data, "220 smtp.localhost.com\n", MAX_BUFFER_SIZE);
    send(client_data->descriptor, server_data->data, MAX_BUFFER_SIZE, 0);
}
int process_client_commands(struct socket_data_struct * server_data, struct socket_data_struct * client_data){ 
    // client_data->data[client_data->data_len] = '\0';
    if(findCommand("HELO", client_data->data)){
        process_HELO((client_data->data + 5), server_data, client_data);
    } 
    if(findCommand("MAIL FROM", client_data->data)){
        process_MAIL_FROM((client_data->data + 10), server_data, client_data);
    }  
    if(findCommand("RCPT TO", client_data->data)){
        process_RCPT_TO((client_data->data + 8), server_data, client_data);
    }  
    if(findCommand("DATA", client_data->data)){
        process_DATA((client_data->data + 5), server_data, client_data);
    }  
    if(findCommand("QUIT", client_data->data)){
        process_QUIT((client_data->data + 5), server_data, client_data);
    }  
    if(findCommand("NOOP", client_data->data)){
        process_NOOP((client_data->data + 5), server_data, client_data);
    }
    if(findCommand("RSET", client_data->data)){
        process_RSET();
    }
}
void process_HELO(char * data, struct socket_data_struct * server_data, struct socket_data_struct * client_data){
    process_RSET();
    receivers = (char **)malloc(MAX_RECEIVERS);
    clientIP = data;
    printf("client IP :%s \n", clientIP);
    strncpy(server_data->data, "250 HELLO \n", MAX_BUFFER_SIZE);
    send_data(client_data->descriptor, server_data->data);
}
void process_MAIL_FROM(char * data, struct socket_data_struct * server_data, struct socket_data_struct * client_data){
    if(clientIP != NULL){
        strcpy(mailFrom, data);
        //getUserAndDomain(data, &user_name, &domain_name);
        printf("MAIL FROM :%s \n", mailFrom);
        strncpy(server_data->data, "250 OK \n", MAX_BUFFER_SIZE);
        send_data(client_data->descriptor, server_data->data);
    }else{
        strncpy(server_data->data, "use HELO command first\n", MAX_BUFFER_SIZE);
        send_data(client_data->descriptor, server_data->data);
    }
}
void process_RCPT_TO(char * data, struct socket_data_struct * server_data, struct socket_data_struct * client_data){  
    if(clientIP != NULL){

        receivers[no_receivers] = (char*)malloc(strlen(data));
        strcpy(receivers[no_receivers], data);
        // receivers[no_receivers][strlen(receivers[no_receivers])] = '\0';

        no_receivers++;
        //for(int i = 0; i < no_receivers; i++)
        printf("Receivers[%d] : %s \n", no_receivers, receivers[no_receivers-1]);

        strncpy(server_data->data, "250 OK \n", MAX_BUFFER_SIZE);
        send_data(client_data->descriptor, server_data->data);

    }else{
        strncpy(server_data->data, "use HELO command first\n", MAX_BUFFER_SIZE);
        send_data(client_data->descriptor, server_data->data);
    }
}
void process_DATA(char * data, struct socket_data_struct * server_data, struct socket_data_struct * client_data){
    if(clientIP != NULL && mailFrom != NULL && receivers[0] != NULL){
        strncpy(server_data->data, "354 Send message content; end with \\n.\\n \n", MAX_BUFFER_SIZE);
        send_data(client_data->descriptor, server_data->data);

        strcpy(client_data->data, "");
        while(strncmp(".\r\n", client_data->data, 2)!=0){
            strcat(email_data, client_data->data);
            client_data->data_len = recv(client_data->descriptor, client_data->data, MAX_BUFFER_SIZE, 0); 
            client_data->data[client_data->data_len] = '\0'; 
        } 
        printf("EMAIL : \n%s", email_data);
        strncpy(server_data->data, "250 OK, message accepted for delivery: \n", MAX_BUFFER_SIZE);
        send_data(client_data->descriptor, server_data->data);

        char * tmpPtr;
        char receipt_user[100];
        char* receipt_domain_name;  
        char filePath[MAX_BUFFER_SIZE] = "./";

        for(int i = 0; i < no_receivers; i++){
            memset(receipt_user, 0, sizeof(receipt_user));
            strcpy(filePath, "./");
            receipt_domain_name = strchr(receivers[i], '@');
            receipt_domain_name++;
            strncpy(receipt_user, receivers[i], strlen(receivers[i]) - strlen(receipt_domain_name) - 1);

            strcat(filePath, receipt_domain_name);
            strcat(filePath,"/");
            strcat(filePath, receipt_user);
            strcat(filePath, ".txt");

            printf("%s\n", filePath);

            if((fptr = fopen(filePath,"a+"))!= NULL){

                if(fprintf(fptr, "*************** \n FROM : %s \n %s \n ***************\n\n", mailFrom, email_data)>0){
                    strncpy(server_data->data, "Delivered Successfully\n", MAX_BUFFER_SIZE);
                    send_data(client_data->descriptor, server_data->data);
                }
                fclose(fptr);

            }else{
                strncpy(server_data->data, "User not exits\n", MAX_BUFFER_SIZE);
                send_data(client_data->descriptor, server_data->data);
            }

        }
 

    }else{
        strncpy(server_data->data, "use MAIL FROM and RCPT TO commands first\n", MAX_BUFFER_SIZE);
        send_data(client_data->descriptor, server_data->data);
    }
   
}
void process_QUIT(char * data, struct socket_data_struct * server_data, struct socket_data_struct * client_data){
    strncpy(server_data->data, "221 BYE\n", MAX_BUFFER_SIZE);
    send_data(client_data->descriptor, server_data->data);
    client_data->data_len = 0;
    //close(client_data->descriptor);
}

void process_NOOP(char * data, struct socket_data_struct * server_data, struct socket_data_struct * client_data){
    strncpy(server_data->data, "250 OK\n", MAX_BUFFER_SIZE);
    send_data(client_data->descriptor, server_data->data);
}

void process_RSET(){
    clientIP = NULL; 
    for(int i = 0; i < no_receivers; i++){
        free(receivers[i]);
    }
    free(receivers);
    no_receivers = 0;
    strcpy(email_data, "");
    strcpy(mailFrom, "");
}

