INCLUDE=-Isrc/
CFLAGS=-g

FILES=$(wildcard src/*.cpp)
OBJS=$(FILES:%.cpp=%.o)

all: compiler

run: compiler
	./compiler

compiler: $(OBJS)
	clang++ -o $@ $(OBJS)
	

.PHONY:
	clean

clean:
	rm -rf src/*.o
	rm -rf compiler

%.o: %.cpp
	clang++ -c $< -o $@ $(INCLUDE) $(CFLAGS)
