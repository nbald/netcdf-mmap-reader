mmap-netcdf-reader
==================

# What is this ?

This is a library for reading data from NetCDF files. It supports only NetCDF3 classic and 64bit offset format.

It's meant to be very efficient, implementing the [mmap](http://en.wikipedia.org/wiki/Mmap) mecanism for automatic and efficient RAM caching by the linux kernel.

Files are mapped to the virtual memory, and so we can retrieve data directly as memory pointer. No more needs for fseek or memory allocation.

As a side effect, this will only works on x86_64 linux. (32bit virtual memory space is too small for working with a lot of files)



# Why not use the well established and stable NetCDF library from UCAR/UNIDATA ?

The NetCDF library is very good for it's job. But there's a lot of overhead when it comes to serving a lot of small random data from very huge datasets.

Using mmap, we can avoid most of the file access and memory allocation operations. So every megabytes of the computer RAM can be used for caching the data.

The newest NetCDF library supports mmap, but it's implemented to fit the previous work. It's not designed for mmap.
