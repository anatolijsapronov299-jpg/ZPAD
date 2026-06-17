#include "Display.hpp"

Display::Display(const std::string& name, FrameProcessor* frameProcessor)
    : windowName(name),
      processor(frameProcessor),
      isDrawing(false),
      startPoint(0, 0),
      currentPoint(0, 0),
      brightnessTrackbarValue(50) {}

void Display::initialize() {
    cv::namedWindow(windowName);

    cv::setMouseCallback(windowName, Display::mouseCallback, this);

    brightnessTrackbarValue = processor->getBrightness();
    cv::createTrackbar(
        "Brightness",
        windowName,
        &brightnessTrackbarValue,
        100,
        Display::brightnessCallback,
        this
    );

    cv::setTrackbarPos("Brightness", windowName, brightnessTrackbarValue);
}

void Display::show(const cv::Mat& frame) {
    cv::Mat output = frame.clone();

    if (isDrawing) {
        cv::Rect previewRect(
            std::min(startPoint.x, currentPoint.x),
            std::min(startPoint.y, currentPoint.y),
            std::abs(startPoint.x - currentPoint.x),
            std::abs(startPoint.y - currentPoint.y)
        );

        cv::rectangle(output, previewRect, cv::Scalar(255, 0, 0), 2);
    }

    cv::imshow(windowName, output);
}

const std::string& Display::getWindowName() const {
    return windowName;
}

void Display::mouseCallback(int event, int x, int y, int flags, void* userdata) {
    Display* display = static_cast<Display*>(userdata);

    if (!display || !display->processor) {
        return;
    }

    if (event == cv::EVENT_LBUTTONDOWN) {
        display->isDrawing = true;
        display->startPoint = cv::Point(x, y);
        display->currentPoint = cv::Point(x, y);
    } else if (event == cv::EVENT_MOUSEMOVE && display->isDrawing) {
        display->currentPoint = cv::Point(x, y);
    } else if (event == cv::EVENT_LBUTTONUP) {
        display->isDrawing = false;
        display->currentPoint = cv::Point(x, y);

        cv::Rect rectangle(
            std::min(display->startPoint.x, display->currentPoint.x),
            std::min(display->startPoint.y, display->currentPoint.y),
            std::abs(display->startPoint.x - display->currentPoint.x),
            std::abs(display->startPoint.y - display->currentPoint.y)
        );

        display->processor->addRectangle(rectangle);
    } else if (event == cv::EVENT_RBUTTONDOWN) {
        display->processor->clearRectangles();
    }
}

void Display::brightnessCallback(int value, void* userdata) {
    Display* display = static_cast<Display*>(userdata);

    if (display && display->processor) {
        display->processor->setBrightness(value);
    }
}
