(*
   Mini Language Compiler (Assembler)

   To run: ocaml str.cma compiler.ml <asm file> <output file>
*)

type line = {num : int; content : string}

let max_oprs = 3
let max_int = 0xff (* Maximum integer supported for this asm language *)
let space = Str.regexp "\\( \\|\t\\)+"
let delim = Str.regexp "\\( \\|\t\\)*,\\( \\|\t\\)*"
let reg = Str.regexp "r\\([0-9]+\\)$"
let imm = Str.regexp "[0-9]+"
let imm_char = Str.regexp "\\'.\\'"

let usage () =
  print_endline "Usage : ocaml str.cma compiler.ml <asm file> <output file>"

let error line msg =
  Printf.printf "[Errror] Line %d : %s\n    %s\n" line.num msg line.content;
  exit 1

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
  | "op_puts" -> 0xc0
  | "op_gets" -> 0xd0
  | opcode -> failwith ("[Unreachable] type_check() should have handled this")

type oprnd_typ = Reg | Imm

let get_operand_type line = function
  | "halt" -> []
  | "load" -> [Reg; Reg]
  | "store" -> [Reg; Reg]
  | "move" -> [Reg; Reg]
  | "puti" -> [Reg; Imm]
  | "add" -> [Reg; Reg; Reg]
  | "sub" -> [Reg; Reg; Reg]
  | "gt" -> [Reg; Reg; Reg]
  | "ge" -> [Reg; Reg; Reg]
  | "eq" -> [Reg; Reg; Reg]
  | "ite" -> [Reg; Imm; Imm]
  | "jump" -> [Imm]
  | "op_puts" -> [Reg]
  | "op_gets" -> [Reg]
  | opcode -> error line ("Invalid opcode: " ^ opcode)

let out_opcode oc opcode =
  opcode_to_int opcode |> output_byte oc

let rec out_no_opr oc cnt =
  if cnt <= 0 then ()
  else (output_byte oc 0; out_no_opr oc (cnt-1))

let out_reg oc num = int_of_string num |> output_byte oc

let out_imm = out_reg

let out_imm_char oc expr = Char.code (String.get expr 1) |> output_byte oc

let out_operand oc operand =
  let o = String.lowercase operand in (* only consider lower cases *)
  if Str.string_match reg o 0 then Str.matched_group 1 o |> out_reg oc
  else if Str.string_match imm o 0 then out_imm oc o
  else if Str.string_match imm_char operand 0 then out_imm_char oc operand
  else failwith ("[Unreachable] type_check() should have handled this")

let out oc opcode operands =
  assert (List.length operands <= 3);
  out_opcode oc opcode;
  List.iter (out_operand oc) operands;
  out_no_opr oc (max_oprs - List.length operands)

let check_range num_str line =
  try
    let num = int_of_string num_str in
    if num < 0 || num > max_int then error line "Invalid integer range"
  with Failure _ -> error line "Invalid integer format"

let check_reg_range = check_range
let check_imm_range = check_range

(* TODO : remove duplication with out_operand() function *)
let check_operand oprnd typ line =
  let oprnd' = String.lowercase oprnd in
  match typ with
  | Reg -> (* expect oprnd is a register *)
    if Str.string_match reg oprnd' 0 then
      check_reg_range (Str.matched_group 1 oprnd') line
    else error line ("Expected Reg operand, but '" ^ oprnd' ^ "' was given")
  | Imm -> (* expect oprnd is an immediate value *)
    if Str.string_match imm oprnd' 0 then check_imm_range oprnd' line
    else if Str.string_match imm_char oprnd' 0 then ()
    else error line ("Expected Imm operand, but '" ^ oprnd' ^ "' was given")

(* Check if the operands have a valid type (reg/imm) for given opcode *)
let type_check opcode operands line =
  let types = get_operand_type line opcode in
  let valid_len = List.length types in
  let rec type_check_aux oprnds typs =
    match oprnds, typs with
    | [], [] -> ()
    | _, [] | [], _ ->
      let error_msg = Printf.sprintf "%s needs %d operands" opcode valid_len in
      error line error_msg
    | oprnd :: oprnd_tail, typ :: typ_tail ->
      check_operand oprnd typ line;
      type_check_aux oprnd_tail typ_tail
  in
  type_check_aux operands types

let parse inpath outpath =
  let ic = open_in inpath in
  let oc = open_out outpath in
  try
    let line_num = ref 0 in
    while true do
      let l = input_line ic in
      line_num := !line_num + 1;
      let line = {num = !line_num; content = l} in
      match Str.split space l with
      | ";" :: _ -> ()
      | [] -> ()
      | opcode :: tail_tokens ->
        let operands = Str.split delim (String.concat "" tail_tokens) in
        type_check opcode operands line;
        out oc opcode operands
    done
  with End_of_file -> (close_in ic; close_out oc)

let main =
  if Array.length Sys.argv <> 3 then (usage(); exit 1)
  else parse Sys.argv.(1) Sys.argv.(2)
