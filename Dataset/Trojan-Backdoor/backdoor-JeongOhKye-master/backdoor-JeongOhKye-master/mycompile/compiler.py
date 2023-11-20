import sys

TotalCode = ''
LabelTable = {}
LineCount = 0

Value1Reg = "r111"
RetReg = "r0" 

def UpdateCode(Str):
    global TotalCode
    global LineCount
    TotalCode += (Str +"\n")
    LineCount += 1

def ProcessASM(Line):
    global LabelTable
    global LineCount
    if Line == '':
        return
    try:
        cmd, args = Line.split(" ",1)
        bl = map(lambda x:x.strip(), args.split(","))
    except:
        UpdateCode(Line)
        return

    if cmd == ':':
        LabelTable[bl[0]] = LineCount

    elif cmd == ';':
        UpdateCode(Line)
        LineCount -= 1
    elif cmd == 'inc':
        reg = bl[0]
        UpdateCode("add %s,%s,%s" % (reg, reg, Value1Reg))
    elif cmd == 'dec':
        reg = bl[0]
        UpdateCode("sub %s,%s,%s" % (reg, reg, Value1Reg))

    elif cmd == 'setStr':
        dstReg = bl[0]
        String = bl[1][1:-1] + "\x00"
        TmpReg = "r255"
        AsciiReg = "r254"
        special = False

        #Temporary Save the address
        UpdateCode("move %s, %s" % (TmpReg,dstReg ))
        for c in String:
            if c == '\\':
                special = True
                continue
            if special and c == 'n':
                c = "\n"
                special = False

            UpdateCode("puti %s, %d" % (AsciiReg,ord(c)))
            UpdateCode("store %s, %s" % (TmpReg, AsciiReg))
            UpdateCode("add %s, %s, %s" % (TmpReg, TmpReg, Value1Reg))
    elif cmd == 'printf':
        srcReg = bl[0]
        UpdateCode("puts %s" % srcReg)
    elif cmd == 'strcmp':
        dstReg = bl[0]
        String = bl[1][1:-1] + "\x00"
        TmpReg = "r255"
        ByteReg = "r251"
        AsciiReg = "r254"
        SumReg = "r253"
        Resultreg = "r252"


        #Temporary Save the address
        UpdateCode("move %s, %s" % (TmpReg,dstReg ))
        UpdateCode("puti %s, 0" % (SumReg))
        for c in String:
            UpdateCode("puti %s, %d" % (AsciiReg,ord(c)))
            UpdateCode("load %s, %s" % (ByteReg, TmpReg)) 
            UpdateCode("eq %s, %s, %s" % (Resultreg, AsciiReg, ByteReg))
            UpdateCode("add %s, %s, %s" % (SumReg, SumReg, Resultreg))
            UpdateCode("add %s, %s, %s" % (TmpReg, TmpReg, Value1Reg))
        UpdateCode("puti r254, %d" % (len(String)))
        UpdateCode("eq %s, r254, %s" % (RetReg, SumReg))
    elif cmd == 'if':
        dstReg = bl[0]
        Truego = bl[1]
        Falsego = bl[2]
        UpdateCode("ite %s, LB_%s, LB_%s" % (dstReg, Truego, Falsego))
    elif cmd == "goto":
        go = bl[0]
        UpdateCode("jump LB_%s" % go)
    else:
        UpdateCode(Line)


def PassLabel():
    global TotalCode
    for label in LabelTable:
        TotalCode = TotalCode.replace("LB_%s" % label, str(LabelTable[label]))


f = open(sys.argv[1], "r")

UpdateCode("puti %s, 1" % (Value1Reg))

Line = f.readline()
while Line != '':
    ProcessASM(Line.strip())
    Line = f.readline()

PassLabel()
print TotalCode

f.close()




        

        

