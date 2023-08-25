#!/bin/bash

cd percona-server

mkdir -p build_debug
cd build_debug
rm ./* -rf

export CPPFLAGS="-DOPTIMIZER_TRACE"

cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DDOWNLOAD_BOOST=1 \
    -DWITH_BOOST=/tmp \
    -DWITH_UNIT_TESTS=OFF

make -j`nproc`
make install

cd ../..
