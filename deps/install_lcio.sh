#!/bin/sh
LCIOVER="02-07-04"
curl -OL https://github.com/iLCSoft/LCIO/archive/v${LCIOVER}.tar.gz
tar xzf v${LCIOVER}.tar.gz
mv LCIO-${LCIOVER} ${HOME}/lcio_bindir/lcio
cd ${HOME}/lcio_bindir/lcio
mkdir build
cd build
cmake ..
make
make install
