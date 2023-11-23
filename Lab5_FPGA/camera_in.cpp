#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include "D8MCapture.h"
#include "hps_0.h"

using namespace cv;
using namespace std;

#ifndef CAPTURE_RAM_DEVICE
#define CAPTURE_RAM_DEVICE "/dev/f2h-dma-memory"
#endif /* ifndef CAPTURE_RAM_DEVICE */

int main(int argc, char* argv[])
{
    Mat src;
    Mat overlay;
    D8MCapture *cap = new D8MCapture(TV_DECODER_TERASIC_STREAM_CAPTURE_BASE, CAPTURE_RAM_DEVICE);
    if (!cap->isOpened()) {
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
    }

    cvtColor(overlay, overlay, COLOR_BGR2BGRA); // changing color channel so that it matches the src channel

    namedWindow("camera");

    while (1) {
        if (!cap->read(src))
            return -1;

        //imwrite("source.jpg", src);

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

        cout << "src: " << src.channels() << " " << src.size() << endl;
        cout << "overlay: " << overlay.channels() << " " << overlay.size() << endl;

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
    delete cap;
    destroyAllWindows();

    return 0;
}
