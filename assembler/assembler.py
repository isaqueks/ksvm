import opcodes
import registers

def le(bytes):
    bytes.reverse()
    return bytes

def append_le(list, bytes):
    bytes.reverse()
    for b in bytes:
        list.append(b)

def get_bytes(number, size = 0):
    size = int(size)
    bytes = []
    while number > 0:
        bytes.append(number & 0xFF)
        number = number >> 8

    while size > 0 and len(bytes) < size:
        bytes.insert(0, 0)
    if size > 0 and len(bytes) > size:
        raise Exception("Number too large for size: " + str(number) + " " + str(size))

    return bytes

class Assembler:

    def __init__(self, out_file, input_file):
        self.out_file = out_file
        self.input_file = input_file
        self.out_bytes = []
        self.labels = {}
        self.indices_to_apply_labels = {}

    def assemble_instruction(self, tokens):
        if len(tokens) == 0:
            return

        byte_arr = []

        tokens.reverse()
        tokens = list(map(lambda x: x.upper(), tokens))
        
        opcode = tokens.pop()

        if opcode not in opcodes.instruction_names:
            raise Exception("Unknown opcode: " + opcode)

        opcode_bin = opcodes.instructions[opcodes.instruction_names.index(opcode)]

        append_le(byte_arr, [0, opcode_bin])

        operand_types = []

        if (opcode.endswith("RI")):
            operand_types.append("R")
            operand_types.append("I")

        elif (opcode.endswith("RR")):
            operand_types.append("R")
            operand_types.append("R")

        elif (opcode.endswith("R")):
            operand_types.append("R")

        elif (opcode.endswith("I")):
            operand_types.append("I")

        operation_size = 32

        if not("LOAD" in opcode and operand_types[1] == "I"):
            if ("16" in opcode):
                operation_size = 16
            elif "8" in opcode:
                operation_size = 8

        operands = []

        for op_type in operand_types:
            if len(tokens) == 0:
                raise Exception("Missing operand for opcode: " + opcode)

            operand = tokens.pop()

            if (op_type == "R"):
                if not registers.is_reg(operand):
                    raise Exception("Expected register operand for opcode: " + opcode)
                append_le(byte_arr, [0, int(registers.get_reg(operand))])
                

            if (op_type == "I"):
                if operand.startswith("R"):
                    raise Exception("Expected immediate operand for opcode: " + opcode)

                imm = 0

                if (operand.startswith(":")):
                    if operation_size != 32:
                        raise Exception("Cannot use label as operand for opcode: " + opcode)

                    self.indices_to_apply_labels[len(self.out_bytes) + len(byte_arr)] = operand[1:]
                    append_le(byte_arr, [0, 0, 0, 0])
                else:
                    if operand.startswith("X"):
                        imm = (int(operand[2:], 16))
                    elif operand.startswith("B"):
                        imm = (int(operand[2:], 2))
                    else:
                        imm = (int(operand))

                    if operation_size == 32:
                        append_le(byte_arr, get_bytes(imm, 4))

                    if operation_size == 16:
                        append_le(byte_arr, get_bytes(imm, 2))

                    if operation_size == 8:
                        append_le(byte_arr, get_bytes(imm, 1))

        return bytearray(byte_arr)
                
    def assemble_tokens(self, tokens):
        if len(tokens) == 1 and tokens[0].endswith(":"):
            self.labels[tokens[0][:-1]] = len(self.out_bytes)
            return []
        else:
            return self.assemble_instruction(tokens)

    def assemble_literal(self, line):
        buffer = ""
        buf_type = None
        reading_buffer = False

        bin = []

        for c in line:

            if c == "\"":
                if not reading_buffer:
                    reading_buffer = True
                    buf_type = "string"
                else:
                    if buf_type != "string":
                        raise Exception("Unexpected string literal")

                    reading_buffer = False
                    buf_type = None
                    bin += bytearray(buffer, "utf-8")
                    buffer = ""
                continue

            elif c.upper() == "X":
                if not reading_buffer:
                    reading_buffer = True
                    buf_type = "hex"
                continue

            elif c.upper() == "B":
                if not reading_buffer:
                    reading_buffer = True
                    buf_type = "bin"
                continue

            elif c == " ":
                if reading_buffer and buf_type != "string":
                    reading_buffer = False
                    buf_type = None
                    if buf_type == "hex":
                        bin += get_bytes(int(buffer, 16), len(buffer) / 2)
                    elif buf_type == "bin":
                        bin += get_bytes(int(buffer, 2), len(buffer) / 8)
                    continue
                elif not reading_buffer:
                    continue

            if reading_buffer:
                buffer += c

        if reading_buffer:
            if buf_type == "hex":
                bin += get_bytes(int(buffer, 16), len(buffer) / 2)
            elif buf_type == "bin":
                bin += get_bytes(int(buffer, 2), len(buffer) / 8)
            elif buf_type == "string":
                bin += bytearray(buffer, "utf-8")

        return bin


    def apply_labels(self):
        for index in self.indices_to_apply_labels:
            label = self.indices_to_apply_labels[index]
            label_index = self.labels[label]
            if label_index is None:
                raise Exception("Unknown label: " + label)

            bin = [
                (label_index >> 24) & 0xFF,
                (label_index >> 16) & 0xFF,
                (label_index >> 8) & 0xFF,
                label_index & 0xFF
            ]

            bin = le(bin)

            self.out_bytes[index] = bin[0]
            self.out_bytes[index + 1] = bin[1]
            self.out_bytes[index + 2] = bin[2]
            self.out_bytes[index + 3] = bin[3]

    # Assemble a single line
    def assemble_line(self, line):
        line = line.strip()

        bin = []
        
        if line.startswith("$"): #raw ascii
            line = line[1:]
            bin = self.assemble_literal(line)
        else:
            if ";" in line:
                line = line.split(";")[0].strip()
            
            if line == "":
                return

            tokens = list(filter(lambda x: len(x) > 0, map(lambda x: x.strip(), line.split(" "))))
            bin = self.assemble_tokens(tokens)

        if (len(bin) > 0):
            for b in bin:
                self.out_bytes.append(b)


    def assemble(self):
        for line in self.input_file:
            self.assemble_line(line)

        self.apply_labels()

        self.out_file.write(bytearray(self.out_bytes))

        