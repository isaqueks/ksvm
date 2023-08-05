
R0 = 0
R1 = 1
R2 = 2
R3 = 3
R4 = 4
R5 = 5
R6 = 6
R7 = 7
R8 = 8
R9 = 9
R10 = 10
R11 = 11
R12 = 12
R13 = 13
R14 = 14
R15 = 15
R16 = 16
R17 = 17
R18 = 18
R19 = 19
R20 = 20
R21 = 21
R22 = 22
R23 = 23
R24 = 24
R25 = 25
R26 = 26
R27 = 27
R28 = 28
R29 = 29
R30 = 30
R31 = 31

registers = [
    "R0",
    "R1",
    "R2",
    "R3",
    "R4",
    "R5",
    "R6",
    "R7",
    "R8",
    "R9",
    "R10",
    "R11",
    "R12",
    "R13",
    "R14",
    "R15",
    "R16",
    "R17",
    "R18",
    "R19",
    "R20",
    "R21",
    "R22",
    "R23",
    "R24",
    "R25",
    "R26",
    "R27",
    "R28",
    "R29",
    "R30",
    "R31"
]

special_names = {
    "EAX": R0,
    "EBX": R1,
    "ECX": R2,
    "EDX": R3,
    "ESP": R26,
    "EBP": R27,
    "EIP": R28,
    "FLAGS": R29
}

def is_reg(reg):
    return reg in registers or reg in special_names

def get_reg(reg):
    if reg in registers:
        return registers.index(reg)
    elif reg in special_names:
        return special_names[reg]
    else:
        raise Exception("Unknown register: " + reg)