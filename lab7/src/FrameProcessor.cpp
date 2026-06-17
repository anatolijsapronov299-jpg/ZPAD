#include "FrameProcessor.hpp"

#include <algorithm>
#include <cmath>

FrameProcessor::FrameProcessor()
    : mode(0), brightness(50), showText(true) {}

void FrameProcessor::setMode(int newMode) {
    mode = newMode;
}

int FrameProcessor::getMode() const {
    return mode;
}

void FrameProcessor::setBrightness(int value) {
    brightness = std::clamp(value, 0, 100);
}

int FrameProcessor::getBrightness() const {
    return brightness;
}

void FrameProcessor::toggleText() {
    showText = !showText;
}

bool FrameProcessor::isTextVisible() const {
    return showText;
}

void FrameProcessor::addRectangle(const cv::Rect& rectangle) {
    if (rectangle.width > 5 && rectangle.height > 5) {
        rectangles.push_back(rectangle);
    }
}

void FrameProcessor::clearRectangles() {
    rectangles.clear();
}

std::string FrameProcessor::getModeName() const {
    switch (mode) {
        case 0: return "Original";
        case 1: return "Color Inversion";
        case 2: return "Gaussian Blur";
        case 3: return "Canny Filter";
        case 4: return "Grayscale";
        case 5: return "Sepia";
        case 6: return "Glitch";
        case 7: return "Cartoon";
        case 8: return "Pixelation";
        case 9: return "Picture in Picture";
        default: return "Unknown";
    }
}

cv::Mat FrameProcessor::applySepia(const cv::Mat& frame) {
    cv::Mat result;
    cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
        0.272, 0.534, 0.131,
        0.349, 0.686, 0.168,
        0.393, 0.769, 0.189
    );

    cv::transform(frame, result, kernel);
    cv::threshold(result, result, 255, 255, cv::THRESH_TRUNC);
    result.convertTo(result, CV_8UC3);

    return result;
}

cv::Mat FrameProcessor::applyGlitch(const cv::Mat& frame) {
    std::vector<cv::Mat> channels;
    cv::split(frame, channels);

    cv::Mat shiftedRed;
    cv::Mat shiftedBlue;

    cv::Mat transformRight = (cv::Mat_<double>(2, 3) << 1, 0, 10, 0, 1, 0);
    cv::Mat transformLeft = (cv::Mat_<double>(2, 3) << 1, 0, -10, 0, 1, 0);

    cv::warpAffine(channels[2], shiftedRed, transformRight, channels[2].size());
    cv::warpAffine(channels[0], shiftedBlue, transformLeft, channels[0].size());

    channels[2] = shiftedRed;
    channels[0] = shiftedBlue;

    cv::Mat result;
    cv::merge(channels, result);
    return result;
}

cv::Mat FrameProcessor::applyCartoon(const cv::Mat& frame) {
    cv::Mat gray;
    cv::Mat edges;
    cv::Mat color;
    cv::Mat result;

    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::medianBlur(gray, gray, 7);
    cv::adaptiveThreshold(gray, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, 2);

    cv::bilateralFilter(frame, color, 9, 75, 75);
    cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);

    cv::bitwise_and(color, edges, result);
    return result;
}

cv::Mat FrameProcessor::applyPixelation(const cv::Mat& frame) {
    cv::Mat small;
    cv::Mat result;

    int width = std::max(1, frame.cols / 20);
    int height = std::max(1, frame.rows / 20);

    cv::resize(frame, small, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);
    cv::resize(small, result, frame.size(), 0, 0, cv::INTER_NEAREST);

    return result;
}

cv::Mat FrameProcessor::applyPictureInPicture(const cv::Mat& frame) {
    cv::Mat result = frame.clone();
    cv::Mat small;

    cv::resize(frame, small, cv::Size(frame.cols / 4, frame.rows / 4));

    int x = frame.cols - small.cols - 20;
    int y = 20;

    if (x >= 0 && y >= 0) {
        small.copyTo(result(cv::Rect(x, y, small.cols, small.rows)));
        cv::rectangle(result, cv::Rect(x, y, small.cols, small.rows), cv::Scalar(255, 255, 255), 2);
    }

    return result;
}

cv::Mat FrameProcessor::process(
    const cv::Mat& frame,
    double fps,
    const std::vector<cv::Rect>& faces,
    bool faceDetectionEnabled,
    bool artificialDelayEnabled
) {
    cv::Mat result;

    switch (mode) {
        case 0:
            result = frame.clone();
            break;

        case 1:
            cv::bitwise_not(frame, result);
            break;

        case 2:
            cv::GaussianBlur(frame, result, cv::Size(21, 21), 0);
            break;

        case 3: {
            cv::Mat gray;
            cv::Mat edges;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::Canny(gray, edges, 50, 150);
            cv::cvtColor(edges, result, cv::COLOR_GRAY2BGR);
            break;
        }

        case 4:
            cv::cvtColor(frame, result, cv::COLOR_BGR2GRAY);
            cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
            break;

        case 5:
            result = applySepia(frame);
            break;

        case 6:
            result = applyGlitch(frame);
            break;

        case 7:
            result = applyCartoon(frame);
            break;

        case 8:
            result = applyPixelation(frame);
            break;

        case 9:
            result = applyPictureInPicture(frame);
            break;

        default:
            result = frame.clone();
            break;
    }

    int brightnessShift = brightness - 50;
    result.convertTo(result, -1, 1.0, brightnessShift);

    for (const auto& rectangle : rectangles) {
        cv::rectangle(result, rectangle, cv::Scalar(0, 255, 0), 2);
    }

    if (faceDetectionEnabled) {
        for (const auto& face : faces) {
            cv::rectangle(result, face, cv::Scalar(0, 0, 255), 2);
            cv::putText(
                result,
                "Face",
                cv::Point(face.x, std::max(20, face.y - 10)),
                cv::FONT_HERSHEY_SIMPLEX,
                0.7,
                cv::Scalar(0, 0, 255),
                2
            );
        }
    }

    if (showText) {
        cv::putText(result, "Mode: " + getModeName(), cv::Point(20, 30),
                    cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 255, 255), 2);

        cv::putText(result, "FPS: " + std::to_string(static_cast<int>(fps)), cv::Point(20, 65),
                    cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 255, 255), 2);

        std::string detectorText = std::string("Face detection: ") + (faceDetectionEnabled ? "ON" : "OFF");
        cv::putText(result, detectorText, cv::Point(20, 100),
                    cv::FONT_HERSHEY_SIMPLEX, 0.65, cv::Scalar(0, 255, 255), 2);

        std::string delayText = std::string("Artificial delay: ") + (artificialDelayEnabled ? "ON" : "OFF");
        cv::putText(result, delayText, cv::Point(20, 130),
                    cv::FONT_HERSHEY_SIMPLEX, 0.65, cv::Scalar(0, 255, 255), 2);

        cv::putText(result, "Keys: 0-9 modes, F face, L lag, T text, Q/Esc exit",
                    cv::Point(20, result.rows - 20),
                    cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(0, 255, 255), 1);
    }

    return result;
}
