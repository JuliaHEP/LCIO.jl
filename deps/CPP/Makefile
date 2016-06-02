all:
	$(CXX) -std=c++11 -I ./ -I ${LCIO}/include/  -L ${LCIO}/lib/ -shared -fPIC -o ../lcio_jl.so lcevt.cc lcrdr.cc lccol.cc lcmcp.cc lcclu.cc lccah.cc lcrcp.cc lcsch.cc lcrel.cc lcsth.cc -llcio -lsio
