#!/bin/bash
FileName=$1
basename=$(basename $FileName)
filename=${basename%.*}  

if ! [ -f "../interpreter/build/interpreter" ] ;then
    echo "Make the interpreter"
    cd ../interpreter
    make
    cd ../test
fi

ocaml str.cma ../compiler/compiler.ml $FileName $filename.minibyte
../interpreter/build/interpreter $filename.minibyte


