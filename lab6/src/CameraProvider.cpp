#include "CameraProvider.hpp"

CameraProvider::CameraProvider(int cameraIndex) {
    camera.open(cameraIndex);
}

bool CameraProvider::isOpened() const {
    return camera.isOpened();
}

bool CameraProvider::getFrame(cv::Mat& frame) {
    if (!camera.isOpened()) {
        return false;
    }

    camera >> frame;
    return !frame.empty();
}
