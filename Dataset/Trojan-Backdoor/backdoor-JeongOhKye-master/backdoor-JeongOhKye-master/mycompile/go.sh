FileName=$1
FN=${FileName##*/}
WithoutExt=${FN%.*}

python compiler.py $FileName  > $WithoutExt.mini
ocaml str.cma ../compiler/compiler.ml $WithoutExt.mini $WithoutExt.minibyte
../interpreter/build/interpreter $WithoutExt.minibyte


