#ifndef FACE_DETECTOR_HPP
#define FACE_DETECTOR_HPP

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <string>
#include <thread>
#include <vector>

class FaceDetector {
private:
    cv::dnn::Net net;

    std::thread worker;
    std::mutex mutex;
    std::condition_variable condition;

    cv::Mat frameForProcessing;
    std::vector<cv::Rect> latestFaces;

    std::atomic<bool> running;
    std::atomic<bool> enabled;
    std::atomic<bool> hasNewFrame;
    std::atomic<bool> artificialDelay;

    double confidenceThreshold;

    void workerLoop();
    std::vector<cv::Rect> detectFaces(const cv::Mat& frame);

public:
    FaceDetector(const std::string& protoPath, const std::string& modelPath);
    ~FaceDetector();

    FaceDetector(const FaceDetector&) = delete;
    FaceDetector& operator=(const FaceDetector&) = delete;

    void start();
    void stop();

    void submitFrame(const cv::Mat& frame);
    std::vector<cv::Rect> getLatestFaces();

    void toggleEnabled();
    bool isEnabled() const;

    void toggleArtificialDelay();
    bool isArtificialDelayEnabled() const;
};

#endif
