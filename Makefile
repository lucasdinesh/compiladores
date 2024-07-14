etapa3: y.tab.c lex.yy.o main.o hash.o ast.o
	gcc -o etapa3 lex.yy.o main.o hash.o ast.o

y.tab.c: parser.y
		yacc parser.y -d

main.o: main.c
	gcc -c main.c

hash.o: hash.c
	gcc -c hash.c

ast.o: ast.c
	gcc -c ast.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

lex.yy.c: scanner.l
	flex --header-file=lex.yy.h scanner.l 
clean:
	rm y.tab.c *.o lex.yy.c y.tab.h lex.yy.h etapa3