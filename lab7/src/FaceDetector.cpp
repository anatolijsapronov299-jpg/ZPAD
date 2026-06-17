#include "FaceDetector.hpp"

#include <chrono>
#include <iostream>

FaceDetector::FaceDetector(const std::string& protoPath, const std::string& modelPath)
    : running(false),
      enabled(false),
      hasNewFrame(false),
      artificialDelay(false),
      confidenceThreshold(0.5) {
    net = cv::dnn::readNetFromCaffe(protoPath, modelPath);

    if (net.empty()) {
        throw std::runtime_error("Cannot load DNN model.");
    }
}

FaceDetector::~FaceDetector() {
    stop();
}

void FaceDetector::start() {
    running = true;
    worker = std::thread(&FaceDetector::workerLoop, this);
}

void FaceDetector::stop() {
    running = false;
    condition.notify_all();

    if (worker.joinable()) {
        worker.join();
    }
}

void FaceDetector::toggleEnabled() {
    enabled = !enabled;
}

bool FaceDetector::isEnabled() const {
    return enabled;
}

void FaceDetector::toggleArtificialDelay() {
    artificialDelay = !artificialDelay;
}

bool FaceDetector::isArtificialDelayEnabled() const {
    return artificialDelay;
}

void FaceDetector::submitFrame(const cv::Mat& frame) {
    if (!enabled || frame.empty()) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mutex);
        frameForProcessing = frame.clone();
        hasNewFrame = true;
    }

    condition.notify_one();
}

std::vector<cv::Rect> FaceDetector::getLatestFaces() {
    std::lock_guard<std::mutex> lock(mutex);
    return latestFaces;
}

void FaceDetector::workerLoop() {
    while (running) {
        cv::Mat localFrame;

        {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait(lock, [this] {
                return hasNewFrame || !running;
            });

            if (!running) {
                break;
            }

            localFrame = frameForProcessing.clone();
            hasNewFrame = false;
        }

        if (localFrame.empty()) {
            continue;
        }

        if (artificialDelay) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        std::vector<cv::Rect> faces = detectFaces(localFrame);

        {
            std::lock_guard<std::mutex> lock(mutex);
            latestFaces = faces;
        }
    }
}

std::vector<cv::Rect> FaceDetector::detectFaces(const cv::Mat& frame) {
    std::vector<cv::Rect> faces;

    cv::Mat blob = cv::dnn::blobFromImage(
        frame,
        1.0,
        cv::Size(300, 300),
        cv::Scalar(104.0, 177.0, 123.0),
        false,
        false
    );

    net.setInput(blob);
    cv::Mat detections = net.forward();

    cv::Mat detectionMatrix(
        detections.size[2],
        detections.size[3],
        CV_32F,
        detections.ptr<float>()
    );

    for (int i = 0; i < detectionMatrix.rows; ++i) {
        float confidence = detectionMatrix.at<float>(i, 2);

        if (confidence > confidenceThreshold) {
            int x1 = static_cast<int>(detectionMatrix.at<float>(i, 3) * frame.cols);
            int y1 = static_cast<int>(detectionMatrix.at<float>(i, 4) * frame.rows);
            int x2 = static_cast<int>(detectionMatrix.at<float>(i, 5) * frame.cols);
            int y2 = static_cast<int>(detectionMatrix.at<float>(i, 6) * frame.rows);

            x1 = std::max(0, std::min(x1, frame.cols - 1));
            y1 = std::max(0, std::min(y1, frame.rows - 1));
            x2 = std::max(0, std::min(x2, frame.cols - 1));
            y2 = std::max(0, std::min(y2, frame.rows - 1));

            int width = x2 - x1;
            int height = y2 - y1;

            if (width > 0 && height > 0) {
                faces.emplace_back(x1, y1, width, height);
            }
        }
    }

    return faces;
}
