#include <iostream>
#include <chrono>

#include "CameraProvider.hpp"
#include "Display.hpp"
#include "FrameProcessor.hpp"
#include "KeyProcessor.hpp"

int main() {
    CameraProvider cameraProvider(0);

    if (!cameraProvider.isOpened()) {
        std::cerr << "Error: Cannot open camera." << std::endl;
        return 1;
    }

    FrameProcessor frameProcessor;
    KeyProcessor keyProcessor;
    Display display("Lab 6 - C++ OpenCV", &frameProcessor);

    display.initialize();

    cv::Mat frame;

    auto previousTime = std::chrono::high_resolution_clock::now();
    double fps = 0.0;

    while (true) {
        if (!cameraProvider.getFrame(frame)) {
            std::cerr << "Error: Cannot read frame from camera." << std::endl;
            break;
        }

        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - previousTime;
        previousTime = currentTime;

        if (elapsed.count() > 0) {
            fps = 1.0 / elapsed.count();
        }

        cv::Mat processedFrame = frameProcessor.process(frame, fps);
        display.show(processedFrame);

        int key = cv::waitKey(1);
        if (!keyProcessor.processKey(key, frameProcessor)) {
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}
