##################################################################################
# STAGE 0 - base environment with first set of runtime dependencies
##################################################################################
FROM centos:centos7 as lhsslutil-s0-base-env
LABEL lhsslutil-stage-base-env="yes"
LABEL lhsslutil-stage-build-env="no"
LABEL lhsslutil-stage-build="no"
LABEL lhsslutil-stage-test-env="no"
LABEL lhsslutil-stage-main="no"

RUN yum -y --enablerepo=extras install epel-release && \
    yum clean all

##################################################################################
# STAGE 1 - build tools and libraries needed to build lhsslutil
##################################################################################
FROM lhsslutil-s0-base-env as lhsslutil-s1-build-env
LABEL lhsslutil-stage-base-env="no"
LABEL lhsslutil-stage-build-env="yes"
LABEL lhsslutil-stage-build="no"
LABEL lhsslutil-stage-test-env="no"
LABEL lhsslutil-stage-main="no"

# for compiling and unit testing
RUN yum -y install \
        cmake3 \
        make \
        gcc \
        gcc-c++ \
        gtest-devel \
        openssl-devel && \
    yum clean all

##################################################################################
# STAGE 2 - the lhsslutil source and compiled binaries
##################################################################################
FROM lhsslutil-s1-build-env as lhsslutil-s2-build
LABEL lhsslutil-stage-base-env="no"
LABEL lhsslutil-stage-build-env="no"
LABEL lhsslutil-stage-build="yes"
LABEL lhsslutil-stage-test-env="no"
LABEL lhsslutil-stage-main="no"

ADD . /lhsslutil
RUN cd /lhsslutil && \
    mkdir ./build && \
    cd ./build && \
    cmake3 \
        -DCMAKE_BUILD_TYPE=Release \
        ../ && \
    make && \
    make test

##################################################################################
# STAGE 3 - the base image with additional built runtime dependencies, lhsslutil 
#           binaries and test binaries needed for running integration tests
#           includes everything from build-env
##################################################################################
FROM lhsslutil-s2-build as lhsslutil-s3-test-env
LABEL lhsslutil-stage-base-env="no"
LABEL lhsslutil-stage-build-env="no"
LABEL lhsslutil-stage-build="no"
LABEL lhsslutil-stage-test-env="yes"
LABEL lhsslutil-stage-main="no"

RUN cd /lhsslutil/build && \
    make install-lhsslutil

##################################################################################
# STAGE 4 - the base image with additional built runtime dependencies and 
#           lhsslutil binaries includes nothing from build-env
##################################################################################
FROM lhsslutil-s0-base-env as lhsslutil-s4-main-env
LABEL lhsslutil-stage-base-env="no"
LABEL lhsslutil-stage-build-env="no"
LABEL lhsslutil-stage-build="no"
LABEL lhsslutil-stage-test-env="no"
LABEL lhsslutil-stage-main="yes"

COPY --from=lhsslutil-s2-build /usr/ /usr/
COPY --from=lhsslutil-s2-build /lhsslutil/ /lhsslutil/
RUN cd /lhsslutil/build && \
    make install-lhsslutil && \
    cd / && \
    rm -rf /lhsslutil
