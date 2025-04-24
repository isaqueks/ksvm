import typing

ENDIANNESS = "little"

def makebytes(num: int, size: int = 0) -> typing.List[int]:
    
    bytes = []
    first = True

    while num > 0 or first:
        bytes.append(num & 0xFF)
        num = num >> 8
        first = False

    while size > 0 and len(bytes) < size:
        bytes.insert(0, 0)
    if size > 0 and len(bytes) > size:
        raise Exception("Number too large for size: " + str(num) + " " + str(size))

    return bytes

def little_endian(bytes: typing.List[int]) -> typing.List[int]:
    return list(reversed(bytes))

def big_endian(bytes: typing.List[int]) -> typing.List[int]:
    return bytes

def correct_endianness(bytes: typing.List[int]) -> typing.List[int]:
    if ENDIANNESS == "little":
        return little_endian(bytes)
    elif ENDIANNESS == "big":
        return big_endian(bytes)
    else:
        raise Exception("Unknown endianness: " + ENDIANNESS)

def validate_bytes(bytes: typing.List[int]) -> None:
    for byte in bytes:
        if byte < 0 or byte > 255:
            raise Exception("Byte out of range: " + str(byte))