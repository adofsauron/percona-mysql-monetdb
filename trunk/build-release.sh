#!/bin/bash

cd percona-server

mkdir -p build_release
cd build_release
rm ./* -rf

export CPPFLAGS="-DOPTIMIZER_TRACE"

cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DDOWNLOAD_BOOST=1 -DWITH_BOOST=/tmp \
    -DWITH_UNIT_TESTS=OFF

make -j`nproc`
make install

cd ../..
