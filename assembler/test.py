import lexer

l = lexer.Lexer()

while True:
    line = input("> ")
    tokens = l.tokenize_line(line)
    for token in tokens:
        print(token)