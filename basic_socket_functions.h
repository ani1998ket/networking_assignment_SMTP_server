#ifndef BASIC_SOCKET_FUNCTIONS
#define BASIC_SOCKET_FUNCTIONS

    #include<netinet/in.h>

    #ifndef MAX_BUFFER_SIZE
    #define MAX_BUFFER_SIZE 1024
    #endif

    #ifndef ERROR
    #define ERROR -1
    #endif

    #ifndef MAX_CLIENTS
    #define MAX_CLIENTS 2
    #endif

    struct socket_data_struct{

        struct sockaddr_in address;
        int descriptor;

        char data[MAX_BUFFER_SIZE];

        
        int data_len;

    };
    

    void init_socket(struct socket_data_struct *);
    void init_socket_address(struct socket_data_struct *, short, in_addr_t );
    void bind_socket(struct socket_data_struct *);
    void listen_socket(struct socket_data_struct *);
    void accept_socket(struct socket_data_struct *, struct socket_data_struct *);
    void connect_socket(struct socket_data_struct *);

#endif 