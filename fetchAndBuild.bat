@echo off
echo This script attempts to fetch and build DGGAL, DGG and its bindings and examples for C, C++, Python and rust.
echo Please make sure you have git installed to fetch the source code from the eC and DGGAL repositories.
echo Please make sure you have GCC (MinGW-w64) or Clang, and GNU Make (mingw32-make) installed.
echo Please make sure you have zlib (header files and library) installed.
echo Please make sure you have GCC or Clang C++ support installed for C++.
echo Please make sure you have the rust compiler (rustc) edition 2024+ installed for rust.
echo Please make sure you have cffi installed for Python (python -m pip install cffi).
echo:
echo Building in 'dgbuild' directory...

mkdir dgbuild
cd dgbuild

echo Fetching eC core development environment...
git clone -b main --single-branch https://github.com/ecere/eC.git

echo Fetching DGGAL...
git clone -b eC-core --single-branch https://github.com/ecere/dggal.git

echo Building eC development environment...
cd eC
mingw32-make -j4

echo Building DGGAL...
cd ..\dggal\
mingw32-make -j4

echo:
echo **************************************
echo ************ DGGAL for eC ************
echo **************************************
echo Execution test for DGG tool:
SET PATH=%PATH%;%cd%\..\eC\obj\win32\bin;%cd%\obj\release.win32;%cd%\obj\win32\bin
obj\release.win32\dgg ISEA3H info A4-0-A
echo Execution test for (static) DGG tool:
obj\static.win32\dgg ISEA3H info A4-0-A

echo:
echo **************************************
echo ************ DGGAL for C *************
echo **************************************
echo Building DGGAL for C...
cd bindings\c
mingw32-make
echo Building DGGAL sample C application...
cd ..\..\bindings_examples\c
mingw32-make
echo Execution test for DGGAL sample C application:
obj\release.win32\info ISEA3H A4-0-A
echo Building DGGAL (static) sample C application...
mingw32-make -f Makefile.static
echo Execution test for DGGAL (static) sample C application:
obj\static.win32\info ISEA3H A4-0-A
cd ..\..

echo:
echo **************************************
echo ************ DGGAL for C++ ***********
echo **************************************
echo Building DGGAL for C++...
cd bindings\cpp
mingw32-make
echo Building DGGAL sample C++ application...
cd ..\..\bindings_examples\cpp
mingw32-make
echo Execution test for DGGAL sample C++ application:
obj\release.win32\info ISEA3H A4-0-A
echo Building DGGAL (static) sample C++ application...
mingw32-make -f Makefile.static
echo Execution test for DGGAL (static) sample C++ application:
obj\static.win32\info ISEA3H A4-0-A
cd ..\..

echo:
echo **************************************
echo *********** DGGAL for rust ***********
echo **************************************
echo Building DGGAL for rust...
cd bindings\rust
mingw32-make
echo Building DGGAL sample rust application...
cd ..\..\bindings_examples\rust
mingw32-make
echo Execution test for DGGAL sample rust application:
obj\win32\info ISEA3H A4-0-A
cd ..\..

echo:
echo **************************************
echo ********** DGGAL for Python **********
echo **************************************
echo Building DGGAL for Python...
cd bindings\py
python build_ecrt.py
python build_dggal.py
echo Execution test for DGGAL sample Python application:
SET PYTHONPATH=%cd%
cd ..\..\bindings_examples\py\
python info.py
cd ..\..

echo:
echo All done! Thank you for trying out and using DGGAL.
