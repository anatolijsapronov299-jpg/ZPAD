#include "KeyProcessor.hpp"

bool KeyProcessor::processKey(int key, FrameProcessor& processor, FaceDetector& detector) {
    if (key == 27 || key == 'q' || key == 'Q') {
        return false;
    }

    if (key >= '0' && key <= '9') {
        processor.setMode(key - '0');
    }

    if (key == 't' || key == 'T') {
        processor.toggleText();
    }

    if (key == 'f' || key == 'F') {
        detector.toggleEnabled();
    }

    if (key == 'l' || key == 'L') {
        detector.toggleArtificialDelay();
    }

    return true;
}
