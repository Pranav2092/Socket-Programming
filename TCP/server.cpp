#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define PORT 8080

using namespace std;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    char buffer[1024] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Socket creation error" << endl;
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        cerr << "Socket reusability error" << endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Socket binding error" << endl;
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        cerr << "Error while listening" << endl;
        return -1;
    }

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                             (socklen_t*)&addrlen)) < 0) {
        cerr << "Error occured in accepting" << endl;
        return -1;
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int valread = read(new_socket, buffer, 1024);
        if (valread <= 0 || strcmp(buffer, "exit") == 0) {
            break;
        }
        cout << "Message from client: " << buffer << endl;
        string reply;
        cout << "Message from server(type 'exit' to stop): ";
        getline(cin, reply);
        send(new_socket, reply.c_str(), reply.length(), 0);
        if(reply == "exit"){
            break;
        }
    }
    close(new_socket);
    close(server_fd);
    return 0;
}
