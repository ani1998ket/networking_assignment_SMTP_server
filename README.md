compile the program using 
    gcc main.c smtp_basic_commands.c basic_socket_functions.c -o server

to run the server use
    ./server "PORT"
    for PORT = 25
        use sudo ./server 25

connect to the server using 
    telnet localhost "PORT"

use following commands in order to send email
(NOTE : currently only two users exists xyz123@example.com and test@example.com)

    HELO <clientIP>
    MAIL FROM <email@domain.com> //your email
    RCPT TO <email@domain.com>  //test@example.com
    RCPT TO <email@domain.com>  //xyz123@example.com
    DATA
    QUIT

    also supported:
    
        RSET
        NOOP 

->currently all emails are saved in  username.txt file under example.com folder