# Instructions for building DGGAL

See also [this script](fetchAndBuild.sh) (running all of these commands), fetching and building everything, and [this batch file](fetchAndBuild.bat) (for the equivalent on Windows).

## Pre-requisites

- ensure git is installed (for fetching the source code)
- ensure zlib is installed, including the "dev" package with header files
- ensure GCC or Clang is installed with working C support
- ensure GNU Make is installed

## Optional build dependenices

- (if wanting to build and use the C++ bindings)         ensure GCC or Clang C++ support is installed
- (if wanting to build Python bindings)                  ensure Python is installed with cffi (pip3 install cffi)
- (if wanting to build rust bindings)                    ensure the rust compiler (rustc) is installed and working
- (if wanting to build smaller compressed executables)   ensure UPX is installed

## Fetching and building

```
mkdir dgbuild
cd dgbuild
git clone -b main --single-branch https://github.com/ecere/eC.git
git clone -b eC-core --single-branch https://github.com/ecere/dggal.git
cd eC
make
cd ../dggal/
make
```

## Running the DGG tool

```
export LD_LIBRARY_PATH=$(pwd)/../eC/obj/linux/lib/:$(pwd)/obj/release.linux
obj/release.linux/dgg ISEA3H info A4-0-A
obj/static.linux/dgg ISEA3H info A4-0-A
```

## C bindings

```
cd bindings/c
make
cd ../../bindings_examples/c
make
obj/release.linux/info ISEA3H A4-0-A
make -f Makefile.static
obj/static.linux/info ISEA3H A4-0-A
cd ../..
```

## C++ bindings (after building the C bindings)

```
cd bindings/cpp
make
cd ../../bindings_examples/cpp
make
obj/release.linux/info ISEA3H A4-0-A
make -f Makefile.static
obj/static.linux/info ISEA3H A4-0-A
cd ../..
```

## rust bindings (after building the C bindings)

```
cd bindings/rust
make
cd ../../bindings_examples/rust
make
obj/linux/info ISEA3H A4-0-A
cd ../..
```

## Python bindings (after building the C bindings)

```
cd bindings/py
python3 build_ecrt.py
python3 build_dggal.py
export PYTHONPATH=$(pwd)
cd ../../bindings_examples/py/
python3 info.py
cd ../..
```
