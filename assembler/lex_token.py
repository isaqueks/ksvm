

class Token:

    value: str
    type: str

    def __init__(self, value: str, type: str):
        self.value = value
        self.type = type

    def __str__(self):
        return self.value + " (" + self.type + ")"

    def numeric_value(self):
        if self.type == "hex":
            return int(self.value, 16)
        elif self.type == "dec":
            return int(self.value, 10)
        elif self.type == "bin":
            return int(self.value, 2)
        else:
            raise Exception("Cannot convert non-numeric token to number")
