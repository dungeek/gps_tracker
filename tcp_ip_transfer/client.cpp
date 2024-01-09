#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

const char* serverIP = "192.168.108.166";  // ESP32's IP address
const int serverPort = 9696;

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

        // Parse latitude and longitude from the received data
        float latitude, longitude;
        if (sscanf(buffer, "Latitude: %f Longitude: %f", &latitude, &longitude) == 2) {
            // Open Google Maps in the web browser (adjust for your system)
            std::string googleMapsUrl = "https://www.google.com/maps?q=" + std::to_string(latitude) + "," + std::to_string(longitude);
            std::string command = "xdg-open \"" + googleMapsUrl + "\"";  // Adjust for your system (xdg-open is for Linux)
            system(command.c_str());
        }

        // Add a delay to avoid continuous rapid requests
        usleep(500000);  // Sleep for 500 milliseconds
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

