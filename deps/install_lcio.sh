#!/bin/sh
curl -OL https://github.com/iLCSoft/LCIO/archive/v02-07-02.tar.gz
tar xzf v02-07-02.tar.gz
mv LCIO-02-07-02 ${HOME}/lcio_bindir/lcio
cd ${HOME}/lcio_bindir/lcio
mkdir build
cd build
cmake ..
make
make install
