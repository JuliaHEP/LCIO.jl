LCIO bindings for Julia
=======================
[![Build Status](https://travis-ci.com/jstrube/LCIO.jl.svg?branch=master)](https://travis-ci.com/jstrube/LCIO.jl)
[![Documentation Status](https://readthedocs.org/projects/lciojl/badge/?version=latest)](https://lciojl.readthedocs.io/en/latest/?badge=latest)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.3666947.svg)](https://doi.org/10.5281/zenodo.3666947)

Introduction
------------

This is a package for reading the LCIO file format, used for studies of the International Linear Collider, and other future collider concepts. See http://lcio.desy.de for details.

Installation Instructions
-------------------------

To install the latest version of the package, use the full path when adding the package. From the julia prompt you can type

```julia
]
add https://github.com/jstrube/LCIO.jl
```

Philosophy
----------

We have attempted to achieve a faithful translation of the C++ API, with method names equal to those documented on the LCIO pages. Nevertheless, attempts have been made to improve the user experience.
Examples:

- All collections are typed, no casting necessary
- Methods that return a `float*` or `double*` in the C++ API return a `Float64[]` on the Julia side.
- Some of the methods on the C++ side returning pointers can return `nullptr`, so need to be wrapped in `if` clauses. The way to deal with this on the julia side is to use something like the following syntax:

 ```julia
 ok, value = getReferencePoint(particle)
 if ok
     println(value)
end
```

- A notable exception is `getPosition` for hits, and `getMomentum` for particles, which we assume always return valid values

Getting Started
---------------

The basic construct for iterating over a file is this:

```julia
using LCIO
LCIO.open("file.slcio") do reader
    for event in reader
        println(getEventNumber(event))
    end
end
```

There are more examples in the `examples/` directory.
