#ifndef KEY_PROCESSOR_HPP
#define KEY_PROCESSOR_HPP

#include "FrameProcessor.hpp"
#include "FaceDetector.hpp"

class KeyProcessor {
public:
    bool processKey(int key, FrameProcessor& processor, FaceDetector& detector);
};

#endif
