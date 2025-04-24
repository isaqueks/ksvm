import opcodes
import registers
import binary as bin
import io
import typing
import asm_lexer
import lex_token
import instruction_meta as imeta
from assemble_exception import AssembleException

class Assembler:

    out_bytes: typing.List[int] = []
    labels: typing.Dict[str, int] = {}
    indices_to_apply_labels: typing.Dict[int, str] = {}
    lexer: asm_lexer.Lexer = asm_lexer.Lexer()

    curr_line_num = 1
    curr_file_name = ""

    def __init__(self, out_file: io.BufferedIOBase, source_file: str):
        self.out_file = out_file
        self.curr_file_name = source_file

    def assemble_instruction(self, tokens: typing.List[lex_token.Token]) -> typing.List[int]:
        instruction_token = tokens.pop(0)

        if instruction_token.type != "instruction":
            raise AssembleException(self.curr_file_name, self.curr_line_num, "Expected instruction, got " + str(tokens[0]))


        bytes: typing.List[int] = []

        meta = imeta.InstructionMeta(instruction_token.value)

        opcode = opcodes.instructions[opcodes.instruction_names.index(instruction_token.value)]

        bytes += bin.correct_endianness(bin.makebytes(opcode, 2))

        for op in meta.opcode_types:

            tok = tokens.pop(0)

            if op == "R":
                if tok.type != "register":
                    raise AssembleException(self.curr_file_name, self.curr_line_num, "Expected register, got " + str(tok))

                regcode = registers.get_reg(tok.value)

                bytes += bin.correct_endianness(bin.makebytes(regcode, 2))

            elif op == "I":
                if tok.type not in ["hex", "dec", "bin", "applied_label"]:
                    raise AssembleException(self.curr_file_name, self.curr_line_num, "Expected raw, got " + str(tok))

                if tok.type == "applied_label":
                    self.indices_to_apply_labels[len(self.out_bytes) + len(bytes)] = tok.value
                    bytes += bin.correct_endianness(bin.makebytes(0, int(meta.operation_size / 8)))
                else:
                    value = tok.numeric_value()

                    if value >= 2 ** meta.operation_size:
                        raise AssembleException(self.curr_file_name, self.curr_line_num, "Value too large")

                    bytes += bin.correct_endianness(bin.makebytes(value, int(meta.operation_size / 8)))

        return bytes
                
    def assemble_tokens(self, tokens: typing.List[lex_token.Token]) -> typing.List[int]:
        if tokens[0].type == "label":
            tok = tokens.pop(0)
            self.labels[tok.value] = len(self.out_bytes)
            if len(tokens) == 0:
                return []
            return self.assemble_tokens(tokens)
        elif tokens[0].type == "instruction":
            return self.assemble_instruction(tokens)
        else:
            return self.assemble_literal(tokens)

    def assemble_literal(self, tokens: typing.List[lex_token.Token]) -> typing.List[int]:
        tok = tokens.pop(0)

        bytes: typing.List[int] = []

        if tok.type != "raw":
            raise AssembleException(self.curr_file_name, self.curr_line_num, "Expected raw, got " + str(tok))

        while len(tokens) > 0:
            tok = tokens.pop(0)

            if tok.type == "string":
                bytes += tok.value.encode("utf-8")
            elif tok.type in ["hex", "dec", "bin"]:
                bytes += bin.correct_endianness(bin.makebytes(tok.numeric_value()))
            else:
                raise AssembleException(self.curr_file_name, self.curr_line_num, "Expected literal, got " + str(tok))

        return bytes


    def apply_labels(self):
        for index in self.indices_to_apply_labels:
            label = self.indices_to_apply_labels[index]
            label_index = self.labels[label]
            if label_index is None:
                raise Exception("Unknown label: " + label)

            bytes = (bin.makebytes(label_index, 0))
            while len(bytes) < 4:
                bytes.append(0)

            print(label_index, label, bytes)
        

            self.out_bytes[index] = bytes[0]
            self.out_bytes[index + 1] = bytes[1]
            self.out_bytes[index + 2] = bytes[2]
            self.out_bytes[index + 3] = bytes[3]

    # Assemble a single line
    def assemble_line(self, line: str) -> None:
        tokens = self.lexer.tokenize_line(line)

        if len(tokens) == 0:
            return
        
        bytes = self.assemble_tokens(tokens)
        if len(tokens) > 0:
            raise AssembleException(self.curr_file_name, self.curr_line_num, "Unexpected token: " + str(tokens[0]))

        if len(bytes) > 0:
            self.out_bytes += bytes


    def assemble_file(self, file_name: str) -> None:

        file = io.open(file_name, "r")

        for line in file:
            self.assemble_line(line)
            self.curr_line_num += 1

        file.close()

    def assemble(self):

        self.assemble_file(self.curr_file_name)

        self.apply_labels()

        bin.validate_bytes(self.out_bytes)

        self.out_file.write(bytearray(self.out_bytes))

        