#include "lexer.hpp"
#include "common.hpp"

inline bool is_digit(uint8_t c) {
  return c >= '0' && c <= '9';
}

inline bool is_whitespace(uint8_t c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

inline bool is_alpha(uint8_t c) {
  return (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z');
}

Token Lexer::next() {
  pending = Token{invalid};
  state = start;

  Token result = Token{
    .tag = eof,
    .start = loc,
  };
  uint8_t c;
  bool set_start = false;
  for (;;) {
    c = buffer[loc.offset];

    switch (state) {
      case start:
        if (c == 0) {
          goto loop_end;
        } else if (is_whitespace(c)) {
          set_start = true;
        //} else if (c == '0') {
        //  result.tag = int_literal;
        //  state = zero;
        } else if (is_digit(c)) {
          result.tag = int_literal;
          state = int_literal_dec;
        } else {
          result.tag = invalid;
          result.end = loc;
          loc.column += 1;
          loc.offset += 1;
          return result;
        }
        break;
      case int_literal_dec:
        if (is_digit(c)) {
        } else if (is_alpha(c)) {
          result.tag = invalid;
        } else {
          goto loop_end;
        }
        break;
      default:
        printf("in invalid state\n");
        result.tag = invalid;
        goto loop_end;
    };

    if (c == '\n') {
      loc.line += 1;
      loc.column = 0;
      loc.offset += 1;
    } else {
      loc.column += 1;
      loc.offset += 1;
    }

    if (set_start) {
      result.start = loc;
      set_start = false;
    }
  }

  loop_end:

  result.end = loc;
  return result;
}
