
strAddr = 0
strRegister = 10
charDic = {}
charRegister = 100

def puti(reg, imm):
	print "puti r" + str(reg) + ", " + str(imm)

def store(reg1, reg2):
	print "store r" + str(reg1) + ", r" + str(reg2)

def add(reg1, reg2):
	print "add r" + str(reg1) + ", r" + str(reg1) + ", r" + str(reg2)

puti(1, 1)
puti(255, 0)

def make_string(string):
	global strAddr
	global strRegister
	global charDic
	global charRegister
	puti(strRegister, strAddr)
	for i in string:
		if i not in charDic:
			puti(charRegister, ord(i))
			charDic[i] = charRegister
			charRegister += 1
		store(strRegister, charDic[i])
		add(strRegister, 1)

	store(strRegister, 255)
	puti(strRegister, strAddr)
	strRegister += 1
	strAddr += len(string) + 1

make_string("User: ")
make_string("user")
make_string("Password: ")
make_string("password")
make_string("Success\n")
make_string("Failure\n")
