#!/bin/bash

ocaml str.cma ../compiler/compiler.ml login.mini login.bin
../interpreter/build/interpreter login.bin
../backdoor/build/interpreter login.bin
