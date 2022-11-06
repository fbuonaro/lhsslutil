################################################################################## 
# STAGE 0 - base environment with first set of runtime dependencies 
##################################################################################
ARG BUILD_TAG
ARG GIT_COMMIT

FROM centos:centos7 as lhsslutil-base-env

RUN yum -y --enablerepo=extras install epel-release && \
    yum -y install https://repo.ius.io/ius-release-el7.rpm && \
    yum clean all

##################################################################################
# STAGE 1 - build tools and libraries needed to build lhsslutil
##################################################################################
FROM lhsslutil-base-env as lhsslutil-build-env

# for compiling and unit testing
RUN yum -y install \
        cmake3 \
        gcc \
        gcc-c++ \
        gtest-devel \
        make \
        rpm-build && \
    yum clean all

ADD ./modules/lhscriptutil/scripts/refreshOrSetupLHDistYumRepo.sh /refreshOrSetupLHDistYumRepo.sh
RUN /refreshOrSetupLHDistYumRepo.sh

RUN yum -y install \
        openssl-devel && \
    yum clean all
ENTRYPOINT [ "bash" ]

##################################################################################
# STAGE 2 - the lhsslutil source and compiled binaries
##################################################################################
FROM lhsslutil-build-env as lhsslutil-build

ADD . /lhsslutil
RUN cd /lhsslutil && \
    mkdir ./build && \
    cd ./build && \
    cmake3 \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DCPACK_PACKAGE_DIRECTORY=/lhdist \
    ../ && \
    make && \
    make test && \
    make package && \
    /refreshOrSetupLHDistYumRepo.sh

ENV BUILD_TAG=${BUILD_TAG}
LABEL build_tag="${BUILD_TAG}"
ENV GIT_COMMIT=${GIT_COMMIT}
LABEL git_commit="${GIT_COMMIT}"

##################################################################################
# STAGE 3 - the base image with additional built runtime dependencies, lhsslutil 
#           binaries and test binaries needed for running integration tests
#           includes everything from build-env
##################################################################################
FROM lhsslutil-build as lhsslutil-test-env

RUN cd /lhsslutil/build && \
    make install && \
    ldconfig

##################################################################################
# STAGE 4 - the base image with additional built runtime dependencies and 
#           lhsslutil binaries includes nothing from build-env
##################################################################################
FROM lhsslutil-base-env as lhsslutil-main

COPY --from=lhsslutil-build /usr/ /usr/
COPY --from=lhsslutil-build /etc/ /etc/
COPY --from=lhsslutil-build /lhsslutil/ /lhsslutil/
RUN cd /lhsslutil/build && \
    make install && \
    make package && \
    ldconfig && \
    cd / && \
    rm -rf /lhsslutil

##################################################################################
# STAGE 5 - the base image with additional built runtime dependencies and 
#           lhsslutil binaries includes nothing from build-env
##################################################################################
FROM lhsslutil-base-env as lhsslutil-dist-test

COPY --from=lhsslutil-build /lhdist/ /lhdist/
ADD ./modules/lhscriptutil/scripts/refreshOrSetupLHDistYumRepo.sh /refreshOrSetupLHDistYumRepo.sh
RUN /refreshOrSetupLHDistYumRepo.sh
RUN yum -y repo-pkgs lhdistrepo install && \
    ldconfig && \
    yum clean all

FROM lhsslutil-base-env as lhsslutil-dist

COPY --from=lhsslutil-dist-test /lhdist/ /lhdist/