gen_code = ''
gen_pc = 0

def puts(string):
	global gen_code, gen_pc
	for i, char in enumerate(string+'\0'):
		if i == 1:
			gen_code += 'puti  r2, 0' + '\n'
			gen_pc += 1
		else:
			gen_code += 'puti  r3, ' + str(ord(char)) + '\n'
                	gen_pc += 1
		gen_code += 'store  r2, r3' + '\n'
                gen_pc += 1
	gen_code += 'puti r2, ' + '0'  + '\n'
        gen_pc += 1
	gen_code += 'puts r2' + '\n'
        gen_pc += 1

def strcmp(string, bad):
	global gen_code, gen_pc
	for i, char in enumerate(string+'\0'):
		if i == 1:
			gen_code += 'puti  r2, 0' + '\n'
        		gen_pc += 1
		else:
			gen_code += 'puti  r3, ' + str(ord(char)) + '\n'
                	gen_pc += 1
                gen_code += 'load  r4, r2' + '\n'
                gen_pc += 1
        	gen_code += 'eq r5, r3, r4' + '\n'
        	gen_pc += 1
		gen_code += 'ite   r5, ' + str(gen_pc + 1) + ', ' + str(bad) + '\n'       	
		gen_pc += 1

gen_code += 'puti  r0, 0' + '\n'
gen_pc += 1
gen_code += 'puti  r1, 1' + '\n'
gen_pc += 1
gen_code += 'jump  63' + '\n' # we jump to the address of the main routine
gen_pc += 1

pc_success = gen_pc
puts("Success\n")
gen_code += 'halt' + '\n'
gen_pc += 1

pc_failure = gen_pc
puts("Failure\n")
gen_code += 'halt' + '\n'
gen_pc += 1

gen_code += '; Start of the main routine' + '\n'

puts("User: ")
gen_code += 'gets r0' + '\n'
gen_pc += 1
strcmp("user", pc_failure)

puts("Password: ")
gen_code += 'gets r0' + '\n'
gen_pc += 1
strcmp("password", pc_failure)

print gen_code



 
