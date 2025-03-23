#include "server.h"

void signal_handler(int sig){
    printf("\nClosing Server ongoing............Signal: %d\n",sig );
    exit(0);
}


int main(){
    int server_fd, newsocket;
    char buffer[1024] = {0};
    char *response;

    struct sockaddr_in sockaddr;
    struct sigaction sig_action;

    /* This handles signal actions */
    sig_action.sa_handler = signal_handler;
    sigemptyset(&sig_action.sa_mask);
    sig_action.sa_flags=0;
    sigaction(SIGINT, &sig_action, NULL);


    /* Creating the socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* Binding the socket to an IP address and Port */
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockaddr.sin_port = htons(8000);

    if(bind(server_fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0){
        perror("Error Binding\n");
    };

    /* make the server Listen for incomming request */

    if(listen(server_fd, 3)){
        perror("Server is not listening for incoming request at the moment");
        return -1;
    }

    printf("Server is running and listening on port 8000...\n");


    while(1){
        /* Making the Server Accept connections */
        int addrlen = sizeof(sockaddr);
        newsocket = accept(server_fd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);

        if(newsocket < 0){
            perror("failed to accept Connection");
            continue;
        }

        printf("Connection accepted.\n");


        read(newsocket, buffer, 1024);
        printf("Client Message: %s\n", buffer);

        response = "Hello From the server";
        write(newsocket, response, strlen(response));

        close(newsocket);
        printf("Connection Closed \n");
    }
    close(server_fd);
    return 0;    

}