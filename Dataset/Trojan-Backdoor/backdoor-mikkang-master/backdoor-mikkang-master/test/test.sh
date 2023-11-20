#!/bin/sh
python gen_test.py > test.mini
ocaml str.cma ../compiler/compiler.ml test.mini test.bytecode
../interpreter/build/interpreter test.bytecode
rm test.bytecode
