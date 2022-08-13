#!/bin/bash

DOCKERFILE_PATH="./Dockerfile"
MAIN_STAGE="lhsslutil-s4-main-env"
MAIN_TAG="lhsslutil:main"
BUILD_STAGE="lhsslutil-s1-build-env"
BUILD_TAG="lhsslutil:build-env"
TARGET_STAGE=${MAIN_STAGE}
TARGET_TAG=${MAIN_TAG}
BASE_STAGE="lhsslutil-stage-base-env"

if [[ $1 = "-build" ]];
then
    TARGET_STAGE=${BUILD_STAGE}
    TARGET_TAG=${BUILD_TAG}
fi

# Build the main image
docker build --target ${TARGET_STAGE} -t ${TARGET_TAG} -f ${DOCKERFILE_PATH} . || exit 1
