#!/bin/bash

cd percona-server

mkdir -p build

dos2unix ./build-ps/*.sh
chmod +x ./build-ps/*.sh

./build-ps/percona-server-8.0_builder.sh --builddir=./build --get_sources=1 --build_src_rpm=1 --build_rpm=1

cd ..
