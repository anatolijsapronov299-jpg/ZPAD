#ifndef CAMERA_PROVIDER_HPP
#define CAMERA_PROVIDER_HPP

#include <opencv2/opencv.hpp>

class CameraProvider {
private:
    cv::VideoCapture camera;

public:
    explicit CameraProvider(int cameraIndex = 0);

    bool isOpened() const;
    bool getFrame(cv::Mat& frame);
};

#endif
