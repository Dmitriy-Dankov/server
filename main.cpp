#include "Server.h"
#include <thread>
#include <unistd.h>
#include <netinet/in.h>

ssize_t my_recv(int socket, char *buffer, size_t length, int flag);

int main(int argc, char *argv[])
{
    //Server server(stoi(argv[1]));
    int Max_Client = 3;

    Server server(Max_Client);
    server.s_Socket(5200);
    server.s_Listen(Max_Client);

    bool toggle = false;

    while(1){
        server.s_newClient( server.s_Accept() );

        if (!toggle) {
            toggle = true;
            thread t( [&]() {
                while (!toggle) usleep(100000);
                toggle = server.s_clientRequestHandler(256, 50000);
            } );
            t.detach();
        }
    }

    return 0;
}

