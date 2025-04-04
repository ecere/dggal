#!/bin/sh
clear
ln -sf ../../bindings/rust/dggal.rs
rustc --edition 2024 info.rs -ldggal_c -lecere_c -lecere
