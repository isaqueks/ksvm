import typing

class InstructionMeta:
    operation_size: int = 32
    opcode_types: typing.List[str] = []

    def __init__(self, instr_name: str):

        if "32" in instr_name:
            self.operation_size = 32
        elif "16" in instr_name:
            self.operation_size = 16
        elif "8" in instr_name:
            self.operation_size = 8

        if instr_name.endswith("RR"):
            self.opcode_types = ["R", "R"]
        elif instr_name.endswith("RI"):
            self.opcode_types = ["R", "I"]
        elif instr_name.endswith("R"):
            self.opcode_types = ["R"]
        elif instr_name.endswith("I"):
            self.opcode_types = ["I"]