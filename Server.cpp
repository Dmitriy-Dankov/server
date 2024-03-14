#include "Server.h"
#include <unistd.h>
#include <fcntl.h>

Server::Server(const int &maxClients)
    : m_SizeClient{maxClients}
{
    m_pListClient = new atomic<int> [m_SizeClient];
}

Server::~Server()
{
    delete m_pListClient;
}

void Server::s_Socket(const int &nPort)
{
    if(!nPort || nPort < 1024 || nPort > 65536) {
        cout << "Invalid port specified!\n";
        exit(1);
    }

    m_nListener = socket(AF_INET, SOCK_STREAM, 0);

    if(m_nListener < 0) {
        cout << "Error: creating socket\n";
        exit(1);
    }

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(nPort);
    m_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(m_nListener, (sockaddr*)&m_addr, sizeof(m_addr)) < 0) {
        cout << "Error: binding socket";
        close(m_nListener);
        exit(1);
    }
}

void Server::s_Listen(int num)
{
    if(listen(m_nListener, num) < 0) {
        cout << "Error: listening socket\n";
        close(m_nListener);
        exit(1);
    }    
}

int Server::s_Accept()
{
    int newSock = accept(m_nListener, 0, 0);
    if(newSock < 0) {
        cout << "Error: accept new socket\n";
        exit(1);
    }

    return newSock;
}

void Server::s_newClient(const int &newSock)
{
    for (auto i = 0; i < m_SizeClient; ++i) {
        if (m_pListClient[i] == 0) {
            ///////////// UNLOCK SOCKET //////////////
            int flags = fcntl(newSock, F_GETFL, 0);
            fcntl(newSock, F_SETFL, flags | O_NONBLOCK);
            /////////////////////////////////////////
            m_pListClient[i] = newSock;
            m_msg = "Welcom to server!\n\0";
            send(m_pListClient[i], &m_msg[0], m_msg.size(), 0);
            ++m_nConnectionCounter;
            break;
        }
    }
}

bool Server::s_clientRequestHandler(size_t bufsize, int microseconds)
{
    char buff[bufsize];
    int szByte;
    while(m_nConnectionCounter) {
        for (int i = 0; i < m_SizeClient; ++i) {
            if (m_pListClient[i] == 0) continue;

            szByte = recv(m_pListClient[i], buff, sizeof(buff), 0);

            if(szByte == -1) {
                usleep(microseconds);
                continue;

            } else if (szByte == 0) {
                    cout << "Client " << m_pListClient[i] << ": Connection to the server is lost...\n";
                    close(m_pListClient[i]);
                    m_pListClient[i] = 0;
                    --m_nConnectionCounter;

            } else cout << "Client " << m_pListClient[i] << ": " << buff << "\n";

            usleep(microseconds);
        }
    }
    return 0;
}
