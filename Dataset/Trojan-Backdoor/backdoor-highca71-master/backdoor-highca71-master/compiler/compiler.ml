(*
   Mini Language Compiler (Assembler)

   To run: ocaml str.cma compiler.ml <asm file>
*)

let max_oprs = 3
let delim = Str.regexp "\\( \\|\t\\)*,\\( \\|\t\\)*"
let reg = Str.regexp "r\\([0-9]+\\)"
let imm = Str.regexp "[0-9]+"

let opcode_to_int = function
  | "halt" -> 0x00
  | "load" -> 0x10
  | "store" -> 0x20
  | "move" -> 0x30
  | "puti" -> 0x40
  | "add" -> 0x50
  | "sub" -> 0x60
  | "gt" -> 0x70
  | "ge" -> 0x80
  | "eq" -> 0x90
  | "ite" -> 0xa0
  | "jump" -> 0xb0
  | "puts" -> 0xc0
  | "gets" -> 0xd0
  | opcode -> failwith ("Invalid opcode: " ^ opcode)

let out_opcode oc opcode =
  opcode_to_int opcode |> output_byte oc

let rec out_no_opr oc cnt =
  if cnt <= 0 then ()
  else (output_byte oc 0; out_no_opr oc (cnt-1))

let out_zero_opr oc opcode =
  out_opcode oc opcode; out_no_opr oc max_oprs

let out_reg oc num = int_of_string num |> output_byte oc

let out_imm = out_reg

let out_operand oc operand =
  let o = String.lowercase operand in (* only consider lower cases *)
  if Str.string_match reg o 0 then Str.matched_group 1 o |> out_reg oc
  else if Str.string_match imm o 0 then out_imm oc o
  else failwith ("Invalid operand: " ^ operand)

let out_one_or_more_oprs oc opcode operands =
  assert (List.length operands <= 3);
  out_opcode oc opcode;
  List.iter (out_operand oc) operands;
  out_no_opr oc (max_oprs - List.length operands)

let out oc opcode operands =
  if String.length operands = 0 then out_zero_opr oc opcode
  else Str.split delim operands |> out_one_or_more_oprs oc opcode

let parse inpath outpath =
  let ic = open_in inpath in
  let oc = open_out outpath in
  let space = Str.regexp "\\( \\|\t\\)+" in
  try
    while true do
      let l = input_line ic in
      match Str.split space l with
      | opcode :: operands -> out oc opcode (String.concat "" operands)
      | _ -> failwith "Str.Split failure"
    done
  with End_of_file -> (close_in ic; close_out oc)

let main =
  if Array.length Sys.argv < 3 then exit 1
  else parse Sys.argv.(1) Sys.argv.(2)
