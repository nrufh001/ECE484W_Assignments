#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include "D8MCapture.h"
#include "hps_0.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

using namespace cv;
using namespace std;

#ifndef CAPTURE_RAM_DEVICE
#define CAPTURE_RAM_DEVICE "/dev/f2h-dma-memory"
#endif /* ifndef CAPTURE_RAM_DEVICE */

/*
Performs a non-blocking reception of the UDP data packets. This means that it won't wait for
data to arrive and instead just checks to see if data is available. This is needed so that
the video feed on the FPGA board can be continuously updated rather then waiting for brightness
data.

@param socketft: The socket file descriptor.
@param buffer: The buffer to store packet received.
@param bufferSize: The size of the buffer.
@param clientAddr: The clients address.
@param clientAddrLen: The length of the clients address.
*/
bool nonBlockingRecv(int socketfd, char* buffer, int bufferSize, sockaddr_in& clientAddr, socklen_t& clientAddrLen) {
    fd_set readSet; // setting up file descriptor
    FD_ZERO(&readSet);
    FD_SET(socketfd, &readSet);

    timeval timeout; // time to wait before looking for data again
    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;  // 10 ms timeout

    int ready = select(socketfd + 1, &readSet, NULL, NULL, &timeout); // makes sure to wait until operations are done so we don't block anything

    if (ready < 0) {
        cerr << "Error in select" << endl;
        return false;
    }

    if (ready > 0 && FD_ISSET(socketfd, &readSet)) { // read data if the socket is ready
        ssize_t bytesRead = recvfrom(socketfd, buffer, bufferSize, 0, (struct sockaddr*)&clientAddr, &clientAddrLen);

        if (bytesRead == -1) {
            cerr << "Error receiving data" << endl;
            return false;
        }

        buffer[bytesRead] = '\0';  // Null-terminate the received data
        return true;
    }

    return false;
}

// Function to process received data and update brightness and contrast
void updateImageParams(int& brightness, int& contrast, const char* buffer) {
    string b = buffer;
    brightness = atoi(b.substr(0, 2).c_str());
    contrast = atoi(buffer + 2);
}

void editImage(Mat& image, int brightness, int contrast) {
    // Image processing
    image.convertTo(image, -1, contrast, brightness);
}

int main() {
    const char* LOCAL_UDP_IP = "0.0.0.0";
    const int LOCAL_UDP_PORT = 80;

    // Create socket
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socketfd == -1) {
        cerr << "Error creating socket" << endl;
        return 1;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(LOCAL_UDP_IP);
    serverAddr.sin_port = htons(LOCAL_UDP_PORT);

    // Bind socket
    if (bind(socketfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Error binding socket" << endl;
        close(socketfd);
        return 1;
    }

    Mat src;
    Mat overlay;
    D8MCapture *cap = new D8MCapture(TV_DECODER_TERASIC_STREAM_CAPTURE_BASE, CAPTURE_RAM_DEVICE);
    if (!cap->isOpened()) {
        close(socketfd);
        return -1;
    }

    char str[100];
    static struct timeval last_time;
    struct timeval current_time;
    static float last_fps;
    float t;
    float fps;

    overlay = imread("images/overlay_image.jpg");
    if (overlay.empty()) {
        cerr << "Error: Could not load overlay image." << endl;
        close(socketfd);
        return -1;
    }

    cvtColor(overlay, overlay, COLOR_BGR2BGRA); // changing color channel so that it matches the src channel

    namedWindow("camera");

    int brightness = 0;
    int contrast = 0;

    while (true) {
        char buffer[4];
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        //cout << "clientAddr: " << clientAddr << endl;
        cout << "clientAddrLen: " << clientAddrLen << endl;
        cout << "buffer: " << buffer << endl;
        cout << "buffer size: " << sizeof(buffer) << endl;
        cout << "socketfd: " << socketfd << endl;

        if (nonBlockingRecv(socketfd, buffer, sizeof(buffer), clientAddr, clientAddrLen)) {
            updateImageParams(brightness, contrast, buffer); // parse the data received
            cout << "Received brightness: " << brightness << ", contrast: " << contrast << endl;
        }

        if (!cap->read(src)) {
            cerr << "Error reading video frame" << endl;
            break;
        }

        editImage(src, brightness, contrast);

        int width = 200;

        // Resize overlay image
        double ar = static_cast<double>(width) / overlay.cols;
        int adjustedHeight = static_cast<int>(overlay.rows * ar);
        resize(overlay, overlay, Size(width, adjustedHeight));

        // Coordinates to place in the top left corner of the image
        int x = 50;
        int y = 50;

        // Region of interest (ROI) on the original image for the overlay image to be placed
        Rect roi(x, y, overlay.cols, overlay.rows);
        Mat mask = overlay.clone(); // creating a mask to copy the overlay into the roi
        mask.convertTo(mask, CV_8U); // converting to 8 bit mask
        overlay.copyTo(src(roi), mask); // copying the overlay to the roi

        gettimeofday(&current_time, NULL);
        t = (current_time.tv_sec - last_time.tv_sec)
                + (current_time.tv_usec - last_time.tv_usec) / 1000000.;
        fps = 1. / t;
        fps = last_fps * 0.8 + fps * 0.2;
        last_fps = fps;
        last_time = current_time;
        sprintf(str, "%2.2f, %2.6f", fps, t);
        putText(src, str, Point(20, 40), FONT_HERSHEY_DUPLEX, 1,
                Scalar(0, 255, 0));

        imshow("camera", src);

        //-- bail out if escape was pressed
        int c = waitKey(10);
        if ((char) c == 27) {
            break;
        }
    }

    close(socketfd);
    delete cap;
    destroyAllWindows();

    return 0;
}
