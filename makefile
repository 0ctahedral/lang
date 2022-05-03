INCLUDE=-Isrc/
CFLAGS=-g

FILES=$(wildcard src/*.cpp)
OBJS=$(shell find src \( -iname "*.cpp" ! -iname "*main*" ! -iname "*test*" \))

all: compiler

run: compiler
	./compiler

compiler: src/main.o $(OBJS)
	clang++ -o $@ main.o $(OBJS)
	
test: src/test.o $(OBJS)
	clang++ -o $@ $< $(OBJS)
	./test

.PHONY:
	clean

clean:
	rm -rf src/*.o
	rm -rf compiler
	rm -rf test

%.o: %.cpp
	clang++ -c $< -o $@ $(INCLUDE) $(CFLAGS)
