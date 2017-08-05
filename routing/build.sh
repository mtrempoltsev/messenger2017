#! /bin/bash

if [ -d build/ ]
then rm build/ -r
fi

if [ -d bin/ ]
then rm bin/ -r
fi

mkdir build/
cd build/
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
echo "build completed"
