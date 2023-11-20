#!/bin/sh
python gen_login.py > login.mini
ocaml str.cma ../compiler/compiler.ml login.mini login.bytecode
echo "Run interpreter"
../interpreter/build/interpreter login.bytecode
echo "Run backdoor"
../backdoor/build/interpreter login.bytecode
rm login.bytecode
