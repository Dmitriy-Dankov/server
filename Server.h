#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <netinet/in.h>
#include <atomic>

using namespace std;

class Server
{
public:
    Server(const int &maxClients = 1);
    ~Server();

    void s_Socket(const int &nPort);
    void s_Listen(int num = 1);
    int s_Accept();
    void s_newClient(const int &newSock);
    bool s_clientRequestHandler(size_t bufsize = 128, int microseconds = 1000);

private:
    struct sockaddr_in m_addr;
    int m_nListener{};
    int m_SizeClient;
    atomic<int> *m_pListClient = nullptr;
    int m_nConnectionCounter{0};

    string m_msg;
};

#endif // SERVER_H
