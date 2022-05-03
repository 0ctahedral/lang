#include <cstdio>
#include <cassert>
#include "lexer.hpp"

int main(int argc, char** argv) {

  Lexer lex = Lexer("");
  assert(lex.next().tag == Tag::eof);
  assert(lex.next().start.line == 0);
  assert(lex.next().start.column == 0);
  assert(lex.next().start.offset == 0);
  assert(lex.next().end.line == 0);
  assert(lex.next().end.column == 0);
  assert(lex.next().end.offset == 0);

  lex = Lexer("5");
  assert(lex.next().tag == Tag::num_int);

  return 0;
}
