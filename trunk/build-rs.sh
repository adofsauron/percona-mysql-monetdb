#!/bin/bash

cd percona-server

mkdir -p build
cd build
rm ./* -rf

export CPPFLAGS="-DOPTIMIZER_TRACE"

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DDOWNLOAD_BOOST=1 -DWITH_BOOST=/tmp \
    -DWITH_UNIT_TESTS=OFF

make -j`nproc`
make install

# dos2unix ./build-ps/*.sh
# chmod +x ./build-ps/*.sh

# ./build-ps/percona-server-8.0_builder.sh --builddir=./build --get_sources=1 --build_src_rpm=1 --build_rpm=1

cd ../..
