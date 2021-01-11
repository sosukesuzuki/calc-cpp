gcc_options = -std=c++17 -Wall --pedantic-errors

calc-cpp: lexer.o parser.o main.o
	g++ $(gcc_options) -o $@ $^

main.o: src/main.cpp
	g++ $(gcc_options) -c $^

parser.o: src/parser.cpp
	g++ $(gcc_options) -c $^

lexer.o: src/lexer.cpp
	g++ $(gcc_options) -c $^

clean:
	rm -f ./calc-cpp
	rm -f ./*.o

.PHONY: clean
