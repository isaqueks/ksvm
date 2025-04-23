from lex_token import Token
from typing import List
import opcodes
import registers

class Lexer:

    def tokenize_line_simple(self, line: str) -> List[Token]:
        line = line.strip()
        result: List[Token] = []

        buffer = ""
        reading_buffer = False
        reading_type = "unknown"

        for c in line:

            if c == "$" and not reading_buffer:
                result.append(Token("$", "raw"))
                continue

            elif c == ";" and reading_type != "string":
                break

            elif c == "\"":
                if not reading_buffer:
                    reading_type = "string"
                    reading_buffer = True
                else:
                    result.append(Token(buffer, "string"))
                    reading_buffer = False
                    reading_type = "unknown"
                    buffer = ""
                continue

            elif not (c == " " or c == '\t') and c.isprintable() and not reading_buffer:
                reading_type = "unknown"
                reading_buffer = True

            elif (c == " " or c == '\t') and not (reading_buffer and reading_type == "string"):
                if len(buffer) > 0:
                    result.append(Token(buffer, reading_type))
                    buffer = ""
                    reading_buffer = False
                    reading_type = "unknown"
                continue

            if reading_buffer:
                buffer += c

        if len(buffer) > 0:
            result.append(Token(buffer, reading_type))

        return result

    def parse_token(self, token: Token) -> Token:
        if token.type != "unknown":
            return token

        if token.value.endswith(":"):
            return Token(token.value[:-1], "label")

        if token.value.startswith(":"):
            return Token(token.value[1:], "applied_label")

        if token.value.upper() in opcodes.instruction_names:
            return Token(token.value.upper(), "instruction")

        if registers.is_reg(token.value.upper()):
            return Token(token.value.upper(), "register")

        if token.value.lower()[0] == "x":
            return Token(token.value.lower()[1:], "hex")

        if token.value.lower()[0] == "b":
            return Token(token.value.lower()[1:], "bin")

        if token.value.isdecimal():
            return Token(token.value, "dec")

        if token.value.startswith("#") and len(token.value) > 1:
            return Token(token.value[1:].upper(), "macro")

        return token

    def parse_tokens(self, tokens: List[Token]) -> List[Token]:
        result: List[Token] = list(map(lambda x: self.parse_token(x), tokens))

        return result

    def tokenize_line(self, line: str) -> List[Token]:
        return self.parse_tokens(self.tokenize_line_simple(line))


