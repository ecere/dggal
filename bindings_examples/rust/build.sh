#!/bin/sh
clear
ln -sf ../../bindings/rust/dggal/
rustc --edition 2024 info.rs -Lnative=../../obj/linux/lib -ldggal_c -lecrt_c -lecrt
