#!/bin/bash

cd $(dirname $0)

root_path=$(pwd)
if [ -d ${root_path}/build ]; then
    rm -rf ${root_path}/build/*
else
    mkdir -p ${root_path}/build
fi
cd ${root_path}/build

cmake ..
make
make install
if [ $? -eq 0 ]; then 
echo "build success"
fi

exit 0

