LCIO bindings for Julia
=======================

Installation Instructions
-------------------------
```
bash
cd /path/to/lcio
source setup.sh
julia
Pkg.clone("https://github.com/jstrube/LCIO.jl")
Pkg.build("LCIO")
```

Installation Instructions for the Cxx branch
--------------------------------------------

```
bash
cd /path/to/lcio
source setup.sh
julia
Pkg.clone("https://github.com/jstrube/LCIO.jl")
Pkg.checkout("LCIO", "Cxx")
```
