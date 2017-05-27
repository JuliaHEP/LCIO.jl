#!/bin/bash
LCIOVER="SOMETHING_WENT_WRONG"
if [[ $# -ge 1 ]]
then
	LCIOVER=${1}
fi
if [[ ! -e "LCIO_LIB/LCIOConfig.cmake" ]]
then
	curl -OL https://github.com/iLCSoft/LCIO/archive/v${LCIOVER}.tar.gz
	tar xzf v${LCIOVER}.tar.gz
	mv LCIO-${LCIOVER} LCIO_LIB
fi
cd LCIO_LIB
mkdir build
cd build
cmake ..
make
make install
