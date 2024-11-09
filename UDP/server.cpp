#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int main() {
    int sock;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUFFER_SIZE];

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        cerr << "Socke tcreation error" << endl;
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sock, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        cerr << "Socket binding error" << endl;
        close(sock);
        return -1;
    }

    socklen_t len;
    int n;

    while (true) {
        len = sizeof(cliaddr);
        n = recvfrom(sock, (char*)buffer, BUFFER_SIZE, MSG_WAITALL,
                     (struct sockaddr*)&cliaddr, &len);
        buffer[n] = '\0';
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        cout << "Message from Client: " << buffer << endl;

        string reply;
        cout << "Message from Server: ";
        getline(cin, reply);
        sendto(sock, reply.c_str(), reply.length(),MSG_CONFIRM, (const struct sockaddr*)&cliaddr,
               len);
        if (reply == "exit") {
            break;
        }
    }
    close(sock);
    return 0;
}
