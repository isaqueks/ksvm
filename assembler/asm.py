
## Simple assembler

import sys
import assembler

source = sys.argv[1]
if source is None:
    print("No source file specified")
    sys.exit(1)

out_path = sys.argv[2]
if out_path is None:
    print("No output file specified")
    sys.exit(1)

# Read source file
with open(source, "r") as f:
    # Iterate over lines
    out = open(out_path, "wb")

    asm = assembler.Assembler(out, f)
    asm.assemble()