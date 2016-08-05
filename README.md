LCIO bindings for Julia
=======================
Build Status: [![Build Status](https://travis-ci.org/jstrube/LCIO.jl.svg?branch=master)](https://travis-ci.org/jstrube/LCIO.jl)

Introduction
------------
This is a package for reading the LCIO file format, used for studies of the International Linear Collider, and other future collider concepts. See http://lcio.desy.de for details.

Prerequisites
-------------
 - The julia programming language: http://julialang.org/
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

Philosophy
----------
We have attempted to achieve a faithful translation of the C++ API, with method names equal to those documented on the LCIO pages. Nevertheless, attempts have been made to improve the user experience.
Examples:
 - All collections are typed, no casting necessary
 - Methods that return a `float*` or `double*` in the C++ API return a `float64[]` instead.
 - Many of the methods on the C++ side returning pointers can return `nullptr`, so need to be wrapped in `if` clauses. The way to deal with this on the julia side is to use something like the following syntax:
 
 ```
 ok, value = getReferencePoint(particle)
 if ok
     println(value)
end
```
 - A notable exception is `getPosition` for hits, and `getMomentum` for particles, which we assume always return valid values

Getting Started
---------------
The basic construct for iterating over a file is this:
```
using LCIO
LCIO.open("file.slcio") do reader
    for event in reader
        println(getEventNumber(event))
    end
end
```
There are more examples in the `examples/` directory.
