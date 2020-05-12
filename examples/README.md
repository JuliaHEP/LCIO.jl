# LCIO Examples

This directory is self-contained. First download the packages needed to run the examples.
These are convenient packages for LCIO data analysis, but they are not dependencies of LCIO itself, so we provide them in a `Project.toml` file. To get the files, start julia with 
```
julia --project=<path/to/this/directory>
] instantiate
```
That's it, now you have the packages.
You can run the files in this directory then with 
```
julia --project=</path/to/this/director> <example.jl>
```
