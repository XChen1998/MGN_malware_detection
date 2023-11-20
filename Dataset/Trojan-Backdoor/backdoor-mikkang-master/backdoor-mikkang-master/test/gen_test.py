g_code = ''
g_pc = 0
def comment(c):
    global g_code
    g_code += c + '\n'

def emit(code):
    global g_code, g_pc
    g_code += code + '\n'
    g_pc += 1

comment("; Test Program for Mini Language Opcode. there are 14 opcodes")
comment("; Simple test begin")
comment("; opcode jump")
emit("jump 2")
emit("halt") #pc = 1; if some test fails, then jump to here
bad = 1
comment("; opcode: puti, gets, puts")
emit("puti r0, 0xff")
emit("gets  r0")
emit("puti r1, 0xff")
emit("puts  r1") #expect: string that I type

comment("; opcode: move")
emit("move r2, r0")
emit("puts r2") #expect: string that I type above again

comment("; opcode: store")
emit("puti r0, 0")
emit("puti r1, 'P'")
emit("store r0, r1")
emit("puts r0") #expect: P

comment("; opcode: load")
emit("puti r0, 0")
emit("puti r1, 0xff")
emit("puts r1") #expect: string that I type above again
emit("load r2, r0")
emit("store r1, r2")
emit("puts r1") #expect: fisrt letter of string above change to 'P'

comment("; opcode: ite")
emit("puti r1, 1")
emit("ite r1, %d, %d" % (g_pc + 1, bad))
emit("puts r0") #expect: P

comment("; From now on, I can use jump, gets, puts, move, puti, store, load, ite")

comment("; opcode: add")
emit("puti r2, 0")
emit("add r2, r2, r1")
emit("ite r2, %d, %d" % (g_pc + 1, bad))
emit("puts r0") #expect: P

comment("; opcode: sub")
emit("puti r2, 1")
emit("sub r2, r2, r1")
emit("ite r2, %d, %d" % (bad, g_pc + 1))
emit("puts r2") #expect: P

comment("; opcode gt")
emit("puti r2, 2")
emit("puti r3, 3")
emit("puti r4, 0xff")
emit("gt r4, r2, r3")
emit("ite r4, %d, %d" % (bad, g_pc + 1))
emit("puti r4, 0xff")
emit("gt r4, r3, r2")
emit("ite r4, %d, %d" % (g_pc + 1, bad))
emit("puti r4, 0xff")
emit("gt r4, r2, r2")
emit("ite r4, %d, %d" % (bad, g_pc + 1))
emit("puts r0") #expect: P

comment("; opcode ge")
emit("puti r2, 2")
emit("puti r3, 3")
emit("puti r4, 0xff")
emit("ge r4, r2, r3")
emit("ite r4, %d, %d" % (bad, g_pc + 1))
emit("puti r4, 0xff")
emit("ge r4, r3, r2")
emit("ite r4, %d, %d" % (g_pc + 1, bad))
emit("puti r4, 0xff")
emit("ge r4, r2, r2")
emit("ite r4, %d, %d" % (g_pc + 1, bad))
emit("puts r0") #expect: P

comment("; opcode eq")
emit("puti r2, 2")
emit("puti r3, 3")
emit("puti r4, 0xff")
emit("eq r4, r2, r3")
emit("ite r4, %d, %d" % (bad, g_pc + 1))
emit("puti r4, 0xff")
emit("eq r4, r3, r2")
emit("ite r4, %d, %d" % (bad, g_pc + 1))
emit("puti r4, 0xff")
emit("eq r4, r2, r2")
emit("ite r4, %d, %d" % (g_pc + 1, bad))
emit("puts r0") #expect: P

comment("; opcode halt")
emit("halt") #expect: exit program successfully not a segmentation fault
emit("puti r0, 0")
emit("puti r1, 'F'")
emit("store r0, r1")
emit("puts r0")
print g_code[:-1]
