#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Flag to control the threads
bool exitFlag = false;

// Processes received data
// https://www.opencv-srf.com/2018/02/change-brightness-of-images-and-videos.html#:~:text=If%20you%20want%20to%20increase,every%20pixel%20in%20the%20image.&text=If%20you%20want%20to%20decrease,every%20pixel%20in%20the%20image
// https://www.opencv-srf.com/2018/02/change-contrast-of-images-and-videos.html#:~:text=In%20order%20to%20increase%20the,positive%20constant%20larger%20that%20one.&text=In%20order%20to%20decrease%20the,positive%20constant%20smaller%20that%20one.
void editImage(int brightness, int contrast) {
    std::cout << "Editing image with brightness: " << brightness << ", contrast: " << contrast << std::endl;
    
    // Image processing
    Mat image = imread("/images/overlay_image.jpg");

        // Check for failure
    if (image.empty())
    {
        cout << "Could not open or find the image" << endl;
    }

    // might need to adjust the 0 - 100 range for contrast so that it's more like 0.0 - 10.0 idk yet tho
    // Mat alteredImage;
    // image.convertTo(alteredImage, -1, contrast, brightness);

}

int main(int argc, char** argv) {
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
