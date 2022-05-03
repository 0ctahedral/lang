#include "lexer.hpp"

inline bool is_digit(uint8_t c) {
  return c >= '0' && c <= '9';
}

inline void Lexer::begin_token(Tag tag) {
  t.start = loc;
  t.tag = tag;
}

Token Lexer::next() {
  t = Token{invalid, {0, 0}};
  state = start;

  for (;;) {
    uint8_t c = buffer[loc.offset];

    switch (state) {
      case start:
        // a normal at the end of the file
        if (c == 0) {
          begin_token(eof);
          return t;
        } else if (is_digit(c)) {
          begin_token(num_int);
          state = int_literal;
        } else {
          t.end = loc;
          t.tag = invalid;
          return t;
        }
        break;
      case int_literal:
        if (c == 0) {
          begin_token(eof);
          return t;
        } else if (is_digit(c)) {
          // do nothing since this is still a number
        } else {
          // TODO: error
        }
        break;

    };

    if (c == '\n') {
      loc.line += 1;
      loc.column = 0;
    } else {
      loc.column += 1;
    }
    loc.offset += 1;
    t.end = loc;
  }
}
