#include <cstdio>
#include <cassert>
#include "lexer.hpp"

void test_lex(const char* name, const char* str, void(*fn)(Lexer* lex)) {
  Lexer lex = {str};
  printf("[%s]", name);
  fn(&lex);
  printf("\n");
}

void test_next(Lexer* lex, Token t) {
  Token n = next(lex);

  if (!(
    n.tag == t.tag &&
    n.start.line   == t.start.line &&
    n.start.column == t.start.column &&
    n.start.offset == t.start.offset &&
    n.end.line   == t.end.line &&
    n.end.column == t.end.column &&
    n.end.offset == t.end.offset
  )) {
      printf("\nexpected tag: %s, start: {%ld, %ld, %ld} end: {%ld, %ld, %ld}\ngot      tag: %s, start: {%ld, %ld, %ld} end: {%ld, %ld, %ld}\n",
          lexeme(t.tag),
          t.start.line,
          t.start.column,
          t.start.offset,
          t.end.line,
          t.end.column,
          t.end.offset,
          lexeme(n.tag),
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


void test_next(Lexer* lex, Tag t) {
  Token n = next(lex);
  if (n.tag != t) {
    printf("\nexpected: %s, got %s\n", lexeme(t), lexeme(n.tag));
    assert(false);
  }
}

void lexer_tests() {
  test_lex("empty", "", [](Lexer* lex){
    test_next(lex, {});
  });

  test_lex("five", "5", [](Lexer* lex){
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

  test_lex("invalid_int_with_letter", "5a", [](Lexer* lex){
    test_next(lex, {
          .tag = Tag::invalid,
          .start = {0, 0, 0},
          .end = {0, 2, 2}
    });
  });

  test_lex("int_with_leading_zero", "05", [](Lexer* lex){
    test_next(lex, {
          .tag = Tag::int_literal,
          .start = {0, 0, 0},
          .end = {0, 2, 2}
    });
  });

  test_lex("float_literal", "0.5", [](Lexer* lex){
    test_next(lex, {
          .tag = Tag::float_literal,
          .start = {0, 0, 0},
          .end = {0, 3, 3}
    });
  });

  test_lex("longer_float_literal", "056.513890", [](Lexer* lex){
    test_next(lex, {
          .tag = Tag::float_literal,
          .start = {0, 0, 0},
          .end = {0, 10, 10}
    });
  });

  test_lex("invalid_float_literal_no_frac", "5.", [](Lexer* lex){
    test_next(lex, {
          .tag = Tag::invalid,
          .start = {0, 0, 0},
          .end = {0, 2, 2}
    });
  });

  test_lex("ints_with_space", "5   7", [](Lexer* lex){
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

  test_lex("ints_with_newline", "53555\n7", [](Lexer* lex){
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

  test_lex("braces_bracks_parens", "{[()]}", [](Lexer* lex){
    test_next(lex, {
          .tag = Tag::l_brace,
          .start = {0, 0, 0},
          .end = {0, 1, 1}
    });
    test_next(lex, {
          .tag = Tag::l_bracket,
          .start = {0, 1, 1},
          .end = {0, 2, 2}
    });
    test_next(lex, {
          .tag = Tag::l_paren,
          .start = {0, 2, 2},
          .end =   {0, 3, 3}
    });
    test_next(lex, {
          .tag = Tag::r_paren,
          .start = {0, 3, 3},
          .end =   {0, 4, 4}
    });
    test_next(lex, {
          .tag = Tag::r_bracket,
          .start = {0, 4, 4},
          .end =   {0, 5, 5}
    });
    test_next(lex, {
          .tag = Tag::r_brace,
          .start = {0, 5, 5},
          .end =   {0, 6, 6}
    });
  });

  test_lex("symbols", "`~!@#$%^&*+-;=:/\\|,.<>?", [](Lexer* lex) {
    test_next(lex, grave);
    test_next(lex, tilde);
    test_next(lex, bang);
    test_next(lex, at);
    test_next(lex, pound);
    test_next(lex, dollar);
    test_next(lex, percent);
    test_next(lex, caret);
    test_next(lex, ampersand);
    test_next(lex, asterisk);
    test_next(lex, plus);
    test_next(lex, minus);
    test_next(lex, semi_colon);
    test_next(lex, equals);
    test_next(lex, colon);
    test_next(lex, slash);
    test_next(lex, backslash);
    test_next(lex, pipe);
    test_next(lex, comma);
    test_next(lex, period);
    test_next(lex, angle_left);
    test_next(lex, angle_right);
    test_next(lex, question_mark);
  });

  test_lex("arithemetic", "4 + 6 * (88 / 9.05)", [](Lexer* lex) {
    test_next(lex, int_literal);
    test_next(lex, plus);
    test_next(lex, int_literal);
    test_next(lex, asterisk);
    test_next(lex, l_paren);
    test_next(lex, int_literal);
    test_next(lex, slash);
    test_next(lex, float_literal);
    test_next(lex, r_paren);
  });

  test_lex("identifier_", "foo_bar134", [](Lexer* lex) {
    test_next(lex, {
          .tag = Tag::identifier,
          .start = {0, 0, 0},
          .end = {0, 10, 10}
    });
  });
  test_lex("identifier-", "foo-bar134", [](Lexer* lex) {
    test_next(lex, {
          .tag = Tag::identifier,
          .start = {0, 0, 0},
          .end = {0, 10, 10}
    });
  });
  test_lex("identifiers", "foo bar134", [](Lexer* lex) {
    test_next(lex, {
          .tag = Tag::identifier,
          .start = {0, 0, 0},
          .end = {0, 3, 3}
    });
    test_next(lex, {
          .tag = Tag::identifier,
          .start = {0, 4, 0},
          .end = {0, 10, 10}
    });
  });
}

int main(int argc, char** argv) {
  // TODO: eof on all these tests
  lexer_tests();

  return 0;
}
