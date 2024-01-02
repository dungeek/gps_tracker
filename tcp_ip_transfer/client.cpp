#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

const char* serverIP = "192.168.108.166";  // Replace with your ESP32's IP address
const int serverPort = 80;

int createSocket() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
    }
    return clientSocket;
}

void connectToServer(int clientSocket) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP, &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(clientSocket);
    }
}

void receiveData(int clientSocket) {
    char buffer[256];
    ssize_t bytesRead;

    while (true) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            std::cerr << "Connection closed by server" << std::endl;
            break;
        }

        buffer[bytesRead] = '\0';
        std::cout << "Received data: " << buffer << std::endl;
    }
}

int main() {
    int clientSocket = createSocket();
    if (clientSocket == -1) {
        return -1;
    }

    connectToServer(clientSocket);
    receiveData(clientSocket);

    close(clientSocket);
    return 0;
}

