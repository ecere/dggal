#!/bin/sh

echo "This script attempts to fetch and build the DGGAL WebAssembly module and its bindings for JavaScript ."
echo "Please make sure you have git installed to fetch the source code from the eC, zlib and DGGAL repositories."
echo "Please make sure you have GCC or Clang installed for your host platform, with 32-bit architecture support."
echo "Please make sure you have the Emscripten SDK installed (confirmed to work with 2.0.34)."
echo "  (git clone https://github.com/emscripten-core/emsdk.git ; cd emsdk ; ./emsdk install 2.0.34 ; ./emsdk activate 2.0.34; source ./emsdk_env.sh)"
echo ""
echo "Building in 'dgbuild' directory..."

mkdir dgbuild
cd dgbuild

echo "Fetching eC core development environment..."
git clone -b main --single-branch https://github.com/ecere/eC.git
cd eC
git submodule update --init --recursive
cd ..

echo "Fetching DGGAL..."
git clone -b main --single-branch https://github.com/ecere/dggal.git

echo "Building eC SDK for WASM..."
cd eC
make -f Makefile.wasm -j4

echo "Building DGGAL for WASM..."
cd ../dggal/
make -f Makefile.wasm -j4

echo ""
echo "All done!"

echo ""
echo "You can find examples of using DGGAL in JavaScript in bindings_examples/js/."
echo "You will need to serve the example files on a Web server together with the following files:"
echo "   dgbuild/dggal/bindings/js/dggal.js"
echo "   dgbuild/dggal/bindings/c_fn/obj/dggal.allinone.linux.wasm/libdggal_c_fn.js.0.0.wasm"
echo "   dgbuild/dggal/bindings/c_fn/obj/dggal.allinone.linux.wasm/libdggal_c_fn.js.0.0.5 (symlinked or renamed as libdggal.js)"

echo "Thank you for trying out and using DGGAL."
