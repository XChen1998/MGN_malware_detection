#!/bin/bash

ocaml str.cma ../compiler/compiler.ml test.mini test.bin
../interpreter/build/interpreter test.bin
