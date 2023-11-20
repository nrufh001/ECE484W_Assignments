#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

// Flag to control the threads
bool exitFlag = false;

// Function to process received data (you can replace this with your actual processing logic)
void editImage(int brightness, int contrast) {
    std::cout << "Editing image with brightness: " << brightness << ", contrast: " << contrast << std::endl;
    // Add your image processing logic here
}

int main() {
    const char* LOCAL_UDP_IP = "0.0.0.0";
    const int LOCAL_UDP_PORT = 80;

    // Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(LOCAL_UDP_IP);
    serverAddr.sin_port = htons(LOCAL_UDP_PORT);

    // Bind socket
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(sockfd);
        return 1;
    }

    // Main loop to receive data
    while (!exitFlag) {
        char buffer[4];
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        ssize_t bytesRead = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);

        if (bytesRead == -1) {
            std::cerr << "Error receiving data" << std::endl;
            continue;
        }

        buffer[bytesRead] = '\0';  // Null-terminate the received data

        if (strcmp(buffer, "END") == 0) {
            exitFlag = true;
        } else {
            int brightness = atoi(buffer);
            int contrast = atoi(buffer + 2);

            editImage(brightness, contrast);

            std::cout << "Received brightness: " << brightness << ", contrast: " << contrast << std::endl;
        }
    }

    close(sockfd);
    return 0;
}
