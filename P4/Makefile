CC=clang++
LEX=lex
YACC=bison
CFLAGS=-I include
YFLAGS=
LFLAGS=

all: bin/compiler

bin/compiler: bin/lexer.o bin/parser.o bin/main.o bin/util.o bin/global.o bin/msgfactory.o
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

bin/main.o: src/main.cpp include/tok.h include/util.h include/global.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -c -o $@ $<

bin/parser.o: src/parser.cpp include/util.h include/global.h src/msgfactory.cpp include/msgfactory.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -c -o $@ $<

bin/lexer.o: src/lexer.cpp include/tok.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -c -o $@ $<

bin/util.o: src/util.cpp include/util.h include/global.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -c -o $@ $<

bin/global.o: src/global.cpp include/global.h 
	@mkdir -p bin
	$(CC) $(CFLAGS) -c -o $@ $<

bin/msgfactory.o: src/msgfactory.cpp include/msgfactory.h include/global.h include/util.h
	@mkdir -p bin
	$(CC) $(CFLAGS) -c -o $@ $<

src/lexer.cpp: config/lexer.l config/parser.y
	$(LEX) $(LFLAGS) -o $@ $<

src/parser.cpp include/tok.h: config/parser.y
	$(YACC) $(YFLAGS) -v --defines=include/tok.h -o src/parser.cpp $<



.PHONY: clean
clean:
	rm -f bin/*.o  bin/compiler
	rm -f src/lexer.cpp src/parser.cpp src/parser.output include/tok.h
	rm -f *.png *.dot
