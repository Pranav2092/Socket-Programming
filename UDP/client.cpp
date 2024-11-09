#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int main() {
    int sock;
    struct sockaddr_in servaddr;
    char buffer[BUFFER_SIZE];

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        cerr << "Socket creation error" << endl;
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len;
    int n;

    while (true) {
        len = sizeof(servaddr);

        string reply;
        cout << "Message from Client: ";
        getline(cin, reply);
        sendto(sock, reply.c_str(), reply.length(), MSG_CONFIRM,
               (const struct sockaddr*)&servaddr, len);
        if (reply == "exit") {
            break;
        }

        n = recvfrom(sock, (char*)buffer, BUFFER_SIZE, MSG_WAITALL,
                     (struct sockaddr*)&servaddr, &len);
        buffer[n] = '\0';
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        cout << "Message from Server: " << buffer << endl;
    }
    close(sock);
    return 0;
}
