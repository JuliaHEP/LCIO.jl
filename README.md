LCIO bindings for Julia
=======================

Prerequisites
-------------
 - A compiler that accepts the --std=c++-14 flag
 - An existing LCIO installation (requirement might be removed in the future)

Installation Instructions
-------------------------
```
bash
cd /path/to/lcio
source setup.sh
julia -e 'Pkg.add("CxxWrap"); Pkg.checkout("CxxWrap"); Pkg.build("CxxWrap")'
julia -e 'Pkg.clone("https://github.com/jstrube/LCIO.jl"); Pkg.build("LCIO")'
```
That's all. 
