#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <opencv2/opencv.hpp>
#include "FrameProcessor.hpp"

class Display {
private:
    std::string windowName;
    FrameProcessor* processor;
    bool isDrawing;
    cv::Point startPoint;
    cv::Point currentPoint;

    static void mouseCallback(int event, int x, int y, int flags, void* userdata);
    static void brightnessCallback(int value, void* userdata);

public:
    Display(const std::string& name, FrameProcessor* frameProcessor);

    void initialize();
    void show(const cv::Mat& frame);
    const std::string& getWindowName() const;
};

#endif
