# Example usage of this Dockerfile:
# (the build-arg arguments are all optional)
#
# 1. cd to Code/MinimalLib/docker
# cd Code/MinimalLib/docker
#
# 2. build the JS and WASM libraries
#    (the build-arg arguments are all optional)
# docker build -t rdkix-minimallib --network=host \
#  --build-arg "RDKIX_GIT_URL=https://github.com/myfork/rdkix.git" \
#  --build-arg "RDKIX_BRANCH=mybranch" .
#
# 3. create a temporary container and copy built libraries
#    from the container to your local filesystem, then destroy
#    the temporary container
# docker create --name=rdkix-minimallib-container rdkix-minimallib:latest --entrypoint /
# docker cp rdkix-minimallib-container:/RDKix_minimal.js ../demo
# docker cp rdkix-minimallib-container:/RDKix_minimal.wasm ../demo
# docker rm rdkix-minimallib-container


ARG RDKIX_GIT_URL="https://github.com/rdkit/rdkit.git"
ARG RDKIX_BRANCH="master"

FROM debian:buster as build-stage
ARG RDKIX_GIT_URL
ARG RDKIX_BRANCH

LABEL maintainer="Greg Landrum <greg.landrum@t5informatics.com>"

RUN apt-get update && apt-get upgrade -y && apt install -y \
  curl \
  wget \
  cmake \
  python3 \
  g++ \
  libeigen3-dev \
  git \
  nodejs

ENV LANG C

WORKDIR /opt
RUN wget -q https://boostorg.jfrog.io/artifactory/main/release/1.67.0/source/boost_1_67_0.tar.gz && \
  tar xzf boost_1_67_0.tar.gz 
WORKDIR /opt/boost_1_67_0
RUN ./bootstrap.sh --prefix=/opt/boost --with-libraries=system && \
  ./b2 install


WORKDIR /opt
RUN git clone https://github.com/emscripten-core/emsdk.git

WORKDIR /opt/emsdk
RUN ./emsdk update-tags && \
  ./emsdk install latest && \
  ./emsdk activate latest

#RUN source ./emsdk_env.sh

RUN mkdir /src
WORKDIR /src
ENV RDBASE=/src/rdkix
RUN git clone ${RDKIX_GIT_URL}
WORKDIR $RDBASE
RUN git fetch --all --tags && \
  git checkout ${RDKIX_BRANCH}

RUN mkdir build
WORKDIR build

RUN echo "source /opt/emsdk/emsdk_env.sh > /dev/null 2>&1" >> ~/.bashrc
SHELL ["/bin/bash", "-c", "-l"]
RUN emcmake cmake -DBoost_INCLUDE_DIR=/opt/boost/include -DRDK_BUILD_FREETYPE_SUPPORT=ON -DRDK_BUILD_MINIMAL_LIB=ON \
  -DRDK_BUILD_PYTHON_WRAPPERS=OFF -DRDK_BUILD_CPP_TESTS=OFF -DRDK_BUILD_INCHI_SUPPORT=ON \
  -DRDK_USE_BOOST_SERIALIZATION=OFF -DRDK_OPTIMIZE_POPCNT=OFF -DRDK_BUILD_THREADSAFE_SSS=OFF \
  -DRDK_BUILD_DESCRIPTORS3D=OFF -DRDK_TEST_MULTITHREADED=OFF \
  -DRDK_BUILD_MAEPARSER_SUPPORT=OFF -DRDK_BUILD_COORDGEN_SUPPORT=ON \
  -DRDK_BUILD_SLN_SUPPORT=OFF -DRDK_USE_BOOST_IOSTREAMS=OFF \
  -DFREETYPE_INCLUDE_DIRS=/opt/emsdk/upstream/emscripten/cache/sysroot/include/freetype2 \
  -DFREETYPE_LIBRARY=/opt/emsdk/upstream/emscripten/cache/sysroot/lib/wasm32-emscripten/libfreetype.a \
  -DCMAKE_CXX_FLAGS="-Wno-enum-constexpr-conversion -s DISABLE_EXCEPTION_CATCHING=0" \
  -DCMAKE_C_FLAGS="-Wno-enum-constexpr-conversion -DCOMPILE_ANSI_ONLY" \
  -DCMAKE_EXE_LINKER_FLAGS="-s MODULARIZE=1 -s EXPORT_NAME=\"'initRDKixModule'\"" ..

# "patch" to make the InChI code work with emscripten:
RUN cp /src/rdkix/External/INCHI-API/src/INCHI_BASE/src/util.c /src/rdkix/External/INCHI-API/src/INCHI_BASE/src/util.c.bak && \
  sed 's/&& defined(__APPLE__)//' /src/rdkix/External/INCHI-API/src/INCHI_BASE/src/util.c.bak > /src/rdkix/External/INCHI-API/src/INCHI_BASE/src/util.c

# build and "install"
RUN make -j2 RDKix_minimal && \
  cp Code/MinimalLib/RDKix_minimal.* ../Code/MinimalLib/demo/

# run the tests
WORKDIR /src/rdkix/Code/MinimalLib/tests
RUN node tests.js

# Copy js and wasm rdkix files to use in browser
# This feature requires the BuildKit backend
# https://docs.docker.com/engine/reference/commandline/build/#custom-build-outputs
FROM scratch as export-stage
COPY --from=build-stage /src/rdkix/Code/MinimalLib/demo /
COPY --from=build-stage /src/rdkix/Code/MinimalLib/docs /
