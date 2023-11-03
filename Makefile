all: testsymtablelist testsymtablehash

testsymtablelist: symtablelist.o testsymtable.o
    gcc -o testsymtablelist symtablelist.o testsymtable.o

testsymtablehash: symtablehash.o testsymtable.o
    gcc -o testsymtablehash symtablehash.o testsymtable.o

symtablelist.o: symtablelist.c symtable.h
    gcc -c symtablelist.c

symtablehash.o: symtablehash.c symtable.h
    gcc -c symtablehash.c