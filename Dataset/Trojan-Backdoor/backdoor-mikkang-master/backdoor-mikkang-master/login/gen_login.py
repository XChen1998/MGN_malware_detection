g_code = ''
g_pc = 0

def comment(c):
    global g_code
    g_code += c + '\n'

def emit(code):
    global g_code, g_pc
    g_code += code + '\n'
    g_pc += 1

def do_puts(addr, string):
    comment('; strcpy(%d, %s); puts(%d)' % (addr, repr(string), addr))
    for i, char in enumerate(string + '\0'):
        if i == 0:
            emit('puti  r2, %d' % addr)
        else:
            emit('add   r2, r2, r1')
        emit('puti  r3, %d' % ord(char))
        emit('store r2, r3')
    emit('puti  r2, %d' % addr)
    emit('puts  r2')

def do_strcmp(addr, string, bad):
    comment('; if (strcmp(%d, %s)) goto %d' % (addr, repr(string), bad))
    for i, char in enumerate(string + '\0'):
        if i == 0:
            emit('puti  r2, %d' % addr)
        else:
            emit('add   r2, r2, r1')
        emit('puti  r3, %d' % ord(char))
        emit('load  r4, r2')
        emit('eq    r5, r3, r4')
        emit('ite   r5, %d, %d' % (g_pc + 1, bad))

comment('; r0 is always 0, r1 is always 1.')
emit("move  r237, r237")
emit('puti  r0, 0')
emit('puti  r1, 1')
emit('jump  64') # address of the main routine.

pc_success = g_pc
do_puts(0, "Success\n")
emit('halt')

pc_failure = g_pc
do_puts(0, "Failure\n")
emit('halt')

# Start of main routine.
#print g_pc

do_puts(0, "User: ")
emit('gets r0')
do_strcmp(0, "user", pc_failure)

do_puts(0, "Password: ")
emit('gets r0')
do_strcmp(0, "password", pc_failure)

emit('jump %d' % pc_success)
print g_code[:-1]
