CC=g++ -std=c++11

all: directory clean binario clear

run: directory clean binario clear execute

binario: regras.o early.o gramatica.o simplificacao.o fnc.o main.o
	$(CC) -o build/binario main.o regras.o early.o gramatica.o simplificacao.o fnc.o

regras.o: regras.cpp regras.h
	$(CC) -c regras.cpp

early.o: early.cpp early.h
	$(CC) -c early.cpp

gramatica.o: gramatica.cpp gramatica.h
	$(CC) -c gramatica.cpp

simplificacao.o: simplificacao.cpp simplificacao.h
	$(CC) -c simplificacao.cpp

fnc.o: fnc.cpp fnc.h
	$(CC) -c fnc.cpp

main.o: main.cpp
	$(CC) -c main.cpp

clear: 
	rm -rf *.o

clean:
	rm -rf *.o
	rm -rf build/binario
	rm -rf build

execute: 
	./build/binario

directory:
	mkdir -p build
