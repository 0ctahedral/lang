#include <cstdio>
#include <cassert>
#include "lexer.hpp"

void test_lex(const char* name, const char* str, void(*fn)(Lexer lex)) {
  Lexer lex = Lexer(str);
  printf("[%s]", name);
  fn(lex);
  printf("\n");
}

void test_next(Lexer& lex, Token t) {
  Token n = lex.next();

  if (!(
    n.tag == t.tag &&
    n.start.line   == t.start.line &&
    n.start.column == t.start.column &&
    n.start.offset == t.start.offset &&
    n.end.line   == t.end.line &&
    n.end.column == t.end.column &&
    n.end.offset == t.end.offset
  )) {
      printf("\nexpected tag: %d, start: {%ld, %ld, %ld} end: {%ld, %ld, %ld}\ngot      tag: %d, start: {%ld, %ld, %ld} end: {%ld, %ld, %ld}\n",
          t.tag,
          t.start.line,
          t.start.column,
          t.start.offset,
          t.end.line,
          t.end.column,
          t.end.offset,
          n.tag,
          n.start.line,
          n.start.column,
          n.start.offset,
          n.end.line,
          n.end.column,
          n.end.offset
      );
      assert(false);
    }
}

int main(int argc, char** argv) {
  test_lex("empty", "", [](Lexer lex){
    test_next(lex, {});
  });

  test_lex("five", "5", [](Lexer lex){
    test_next(lex, {
          .tag = Tag::int_literal,
          .start = {0, 0, 0},
          .end = {0, 1, 1}
    });
    test_next(lex, {
          .tag = Tag::eof,
          .start = {0, 1, 1},
          .end = {0, 1, 1}
    });
  });

  test_lex("int_with_letter", "5a", [](Lexer lex){
    test_next(lex, {
          .tag = Tag::invalid,
          .start = {0, 0, 0},
          .end = {0, 2, 2}
    });
  });

  test_lex("ints_with_space", "5   7", [](Lexer lex){
    test_next(lex, {
          .tag = Tag::int_literal,
          .start = {0, 0, 0},
          .end = {0, 1, 1}
    });
    test_next(lex, {
          .tag = Tag::int_literal,
          .start = {0, 4, 4},
          .end = {0, 5, 5}
    });
  });

  test_lex("ints_with_newline", "53555\n7", [](Lexer lex){
    test_next(lex, {
          .tag = Tag::int_literal,
          .start = {0, 0, 0},
          .end = {0, 5, 5}
    });
    test_next(lex, {
          .tag = Tag::int_literal,
          .start = {1, 0, 6},
          .end = {1, 1, 7}
    });
  });

  return 0;
}
