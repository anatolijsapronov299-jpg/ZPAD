#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class FrameProcessor {
private:
    int mode;
    int brightness;
    bool showText;
    std::vector<cv::Rect> rectangles;

    cv::Mat applySepia(const cv::Mat& frame);
    cv::Mat applyGlitch(const cv::Mat& frame);
    cv::Mat applyCartoon(const cv::Mat& frame);
    cv::Mat applyPixelation(const cv::Mat& frame);
    cv::Mat applyPictureInPicture(const cv::Mat& frame);

public:
    FrameProcessor();

    void setMode(int newMode);
    int getMode() const;

    void setBrightness(int value);
    int getBrightness() const;

    void toggleText();
    bool isTextVisible() const;

    void addRectangle(const cv::Rect& rectangle);
    void clearRectangles();

    cv::Mat process(const cv::Mat& frame, double fps, const std::vector<cv::Rect>& faces, bool faceDetectionEnabled, bool artificialDelayEnabled);

    std::string getModeName() const;
};

#endif
