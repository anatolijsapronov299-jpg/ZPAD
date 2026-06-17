#include "KeyProcessor.hpp"

bool KeyProcessor::processKey(int key, FrameProcessor& processor) {
    if (key == 27 || key == 'q' || key == 'Q') {
        return false;
    }

    if (key >= '0' && key <= '9') {
        processor.setMode(key - '0');
    }

    if (key == 't' || key == 'T') {
        processor.toggleText();
    }

    return true;
}
