#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define PORT 8080

using namespace std;

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Socket creation error" << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cerr << "Invalid address / Address not supported" << endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Error connecting" << endl;
        return -1;
    }

    while (true) {
        string reply;
        cout << "Message from client(type 'exit to stop): ";
        getline(cin, reply);
        send(sock, reply.c_str(), reply.length(), 0);
        if (reply == "exit") {
            break;
        }
        memset(buffer, 0, sizeof(buffer));
        int valread = read(sock, buffer, 1024);
        if (valread <= 0 || strcmp(buffer, "exit") == 0) {
            break;
        }
        cout << "Message from server: " << buffer << endl;
    }
    close(sock);
    return 0;
}
