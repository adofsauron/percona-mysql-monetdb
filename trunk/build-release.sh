#!/bin/bash

cd percona-server

rm -rf build_release

mkdir -p build_release
cd build_release
rm ./* -rf

export CPPFLAGS="-DOPTIMIZER_TRACE"

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DDOWNLOAD_BOOST=1 \
    -DWITH_BOOST=/tmp \
    -DWITH_UNIT_TESTS=OFF

make -j`nproc`
make install

cd ../..
