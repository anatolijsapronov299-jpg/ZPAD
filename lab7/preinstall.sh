#!/bin/bash

set -e

echo "Installing dependencies for Lab 7..."
sudo apt update
sudo apt install -y libopencv-dev cmake g++ build-essential wget

mkdir -p models

PROTO_URL="https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt"
MODEL_URL="https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel"

if [ ! -f models/deploy.prototxt ]; then
    echo "Downloading deploy.prototxt..."
    wget -O models/deploy.prototxt "$PROTO_URL"
else
    echo "deploy.prototxt already exists."
fi

if [ ! -f models/res10_300x300_ssd_iter_140000.caffemodel ]; then
    echo "Downloading Caffe model..."
    wget -O models/res10_300x300_ssd_iter_140000.caffemodel "$MODEL_URL"
else
    echo "Caffe model already exists."
fi

echo "Dependencies and model files are ready."
