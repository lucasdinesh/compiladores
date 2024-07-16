#  Etapa 3 - Compiladores 2024/1
#      Autor: Lucas Dinesh

etapa3: y.tab.o lex.yy.o main.o hash.o ast.o
	gcc main.o lex.yy.o y.tab.o hash.o ast.o -o etapa3
main.o: main.c
	gcc -c main.c
hash.o: hash.c
	gcc -c hash.c
ast.o:	ast.c
	gcc -c ast.c
y.tab.o: y.tab.c
	gcc -c y.tab.c
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
y.tab.c: parser.y
	yacc parser.y -d -v
lex.yy.c: scanner.l
	lex scanner.l 
clean:
	rm *.o lex.yy.c lex.yy.h y.tab.c y.output y.tab.h etapa3