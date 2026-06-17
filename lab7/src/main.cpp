#include <chrono>
#include <iostream>

#include "CameraProvider.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"
#include "FrameProcessor.hpp"
#include "KeyProcessor.hpp"

int main() {
    const std::string protoPath = "models/deploy.prototxt";
    const std::string modelPath = "models/res10_300x300_ssd_iter_140000.caffemodel";

    CameraProvider cameraProvider(0);

    if (!cameraProvider.isOpened()) {
        std::cerr << "Error: Cannot open camera." << std::endl;
        return 1;
    }

    FrameProcessor frameProcessor;
    KeyProcessor keyProcessor;
    Display display("Lab 7 - Threaded Face Detection", &frameProcessor);

    FaceDetector faceDetector(protoPath, modelPath);
    faceDetector.start();

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

        faceDetector.submitFrame(frame);
        std::vector<cv::Rect> faces = faceDetector.getLatestFaces();

        cv::Mat processedFrame = frameProcessor.process(
            frame,
            fps,
            faces,
            faceDetector.isEnabled(),
            faceDetector.isArtificialDelayEnabled()
        );

        display.show(processedFrame);

        int key = cv::waitKey(1);
        if (!keyProcessor.processKey(key, frameProcessor, faceDetector)) {
            break;
        }
    }

    faceDetector.stop();
    cv::destroyAllWindows();

    return 0;
}
