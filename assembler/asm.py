
## Simple assembler

import sys
import assembler
import io
from assemble_exception import AssembleException

source = sys.argv[1]
if source is None:
    print("No source file specified")
    sys.exit(1)

out_path = sys.argv[2]
if out_path is None:
    print("No output file specified")
    sys.exit(1)

    # Iterate over lines
out = io.open(out_path, "wb")

try:
    asm = assembler.Assembler(out, source)
    asm.assemble()

except AssembleException as e:
    print("Error: " + str(e))

except Exception as e:
    print("Unknown error: " + str(e))

finally:
    out.close()