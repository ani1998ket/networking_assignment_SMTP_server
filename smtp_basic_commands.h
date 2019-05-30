#ifndef SMTP_BASIC_COMMANDS
#define SMTP_BASIC_COMMANDS
    #include "basic_socket_functions.h"
    int init_SMTP(struct socket_data_struct *, struct socket_data_struct * );
    int process_client_commands(struct socket_data_struct *, struct socket_data_struct *  );
    void process_HELO(char *, struct socket_data_struct *, struct socket_data_struct * );
    void process_MAIL_FROM(char *, struct socket_data_struct *, struct socket_data_struct * );
    void process_RCPT_TO(char *, struct socket_data_struct *, struct socket_data_struct * );
    void process_DATA(char *, struct socket_data_struct *, struct socket_data_struct * );
    void process_QUIT(char *, struct socket_data_struct *, struct socket_data_struct * );
    void process_NOOP(char *, struct socket_data_struct *, struct socket_data_struct * );
    void process_RSET( );
#endif